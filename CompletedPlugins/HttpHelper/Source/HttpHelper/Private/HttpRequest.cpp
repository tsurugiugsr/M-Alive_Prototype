// Copyright 2021 RLoris


#include "HttpRequest.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "TimerManager.h"

UHttpRequest* UHttpRequest::AsyncHttpRequest(UObject* WorldContextObject, const FHttpRequestOptions& Options)
{
	UHttpRequest* Node = NewObject<UHttpRequest>();
	Node->Active = false;
	Node->WorldContextObject = WorldContextObject;
	Node->Options = Options;
	// not garbage collected
	Node->AddToRoot();
	return Node;
}

void UHttpRequest::Activate()
{
	if (nullptr == this->WorldContextObject)
	{
		FFrame::KismetExecutionMessage(TEXT("Invalid WorldContextObject. Cannot execute HttpRequest"), ELogVerbosity::Error);
		this->_Failed(EHttpRequestError::Activation, "Invalid WorldContextObject. Cannot execute HttpRequest");
		return;
	}
	if (this->Active)
	{
		FFrame::KismetExecutionMessage(TEXT("HttpRequest is already running, cancelling now and retrying"), ELogVerbosity::Warning);
		// cancel request and retry
		this->HttpRequest->CancelRequest();
		this->Active = false;
	}
	// Prepare response holder
	FHttpResponse ResponseHttp;
	this->HttpResponse = ResponseHttp;
	// Create HTTP Request
	this->HttpRequest = FHttpModule::Get().CreateRequest();
	// Verb / Action
	FString StrMethod = UEnum::GetValueAsString(this->Options.Method);
	FString Verb;
	StrMethod.Split("::", &StrMethod, &Verb);
	this->HttpRequest->SetVerb(Verb.ToUpper());
	// Headers
	for (TPair<FString,FString> Header : this->Options.Headers)
	{
		this->HttpRequest->SetHeader(Header.Key, Header.Value);
	}
	// Url
	this->HttpRequest->SetURL(this->Options.Url);
	// Set custom timeout
	// this->HttpRequest->SetTimeout(this->Options.Timeout);
	// body
	if (this->Options.BodyType != EBodyType::None)
	{
		// Content type header
		if (this->Options.ContentType != EHttpContentType::Custom)
		{
			FString ContentType = UEnum::GetDisplayValueAsText(this->Options.ContentType).ToString();
			// fix android
			if (ContentType.Contains(TEXT("_")))
			{
				ContentType.ReplaceInline(TEXT("____"), TEXT("."));
				ContentType.ReplaceInline(TEXT("___"), TEXT("+"));
				ContentType.ReplaceInline(TEXT("__"), TEXT("-"));
				ContentType.ReplaceInline(TEXT("_"), TEXT("/"));
				ContentType.ToLowerInline();
			}
			this->HttpRequest->SetHeader(TEXT("Content-Type"), ContentType);
		}
		// check content type for body
		if (this->HttpRequest->GetHeader(TEXT("Content-Type")).IsEmpty())
		{
			this->_Failed(EHttpRequestError::InvalidRequest, "Content-Type header is missing, cannot complete HttpRequest");
			return;
		}
		switch (this->Options.BodyType) 
		{
			case(EBodyType::Bytes):
				this->HttpRequest->SetContent(this->Options.BytesBody);
				break;
			case(EBodyType::Text):
				this->HttpRequest->SetContentAsString(this->Options.TextBody);
				break;
			case(EBodyType::File):
				if (!this->HttpRequest->SetContentAsStreamedFile(this->Options.FilepathBody))
				{
					FFrame::KismetExecutionMessage(TEXT("Filepath for payload is invalid, cannot complete HttpRequest"), ELogVerbosity::Error);
					this->_Failed(EHttpRequestError::InvalidBody, "Filepath for payload is invalid, cannot complete HttpRequest");
					return;
				}
				break;
		}
	}
	// callback progress
	this->HttpRequest->OnRequestProgress().BindLambda([this](FHttpRequestPtr Request, int32 SentBytes, int32 ReceivedBytes)
	{
		this->HttpResponse.BytesReceived = ReceivedBytes;
		this->HttpResponse.BytesSent = SentBytes;
		// upload
		int32 UploadLength = Request->GetContent().Num();
		if (UploadLength != 0)
		{
			this->HttpResponse.UploadPercentage = ( (float)SentBytes / (float)UploadLength) * 100.0f;
		}
		// download
		if (Request->GetResponse().IsValid())
		{
			int32 DownloadLength = Request->GetResponse()->GetContentLength();
			if (DownloadLength != 0)
			{	
				this->HttpResponse.DownloadPercentage = ((float)ReceivedBytes / (float)DownloadLength) * 100.0f;
			}
		}
		this->_Progress();
	});
	// callback headers
	this->HttpRequest->OnHeaderReceived().BindLambda([this](FHttpRequestPtr Request, const FString& Key, const FString& Value)
	{
		this->HttpResponse.Headers.Add(Key, Value);
	});
	// callback complete
	this->HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		if (!Request.IsValid() || !Response.IsValid()) 
		{
			// handle fail
			FFrame::KismetExecutionMessage(TEXT("Request or response is invalid, cannot complete HttpRequest"), ELogVerbosity::Error);
			this->_Failed(EHttpRequestError::InvalidResponse, "Request or response is invalid, cannot complete HttpRequest");
			return;
		}
		this->HttpResponse.Code = Response->GetResponseCode();
		this->HttpResponse.ContentType = Response->GetContentType();
		this->HttpResponse.ElapsedTime = Request->GetElapsedTime();
		this->HttpResponse.Status = EHttpRequestStatus::ToString(Request->GetStatus());
		this->HttpResponse.IsFinished = EHttpRequestStatus::IsFinished(Request->GetStatus());
		this->HttpResponse.IsResponseCodeOk = EHttpResponseCodes::IsOk(Response->GetResponseCode());
		this->HttpResponse.Content = Response->GetContentAsString();
		this->HttpResponse.RawContent = Response->GetContent();
		if (bSuccess)
		{
			this->_Completed();
		} 
		else
		{
			EHttpRequestError ErrorReason = TimeoutSeconds == 0.f ? EHttpRequestError::TimedOut : EHttpRequestError::FailedRequest;
			FString ErrorMsg = TimeoutSeconds == 0.f ? "HttpRequest: canceled because timed out" : "";
			this->_Failed(ErrorReason, ErrorMsg);
		}
	});
	// start request
	TimeoutSeconds = this->Options.Timeout > 0 ? static_cast<float>(this->Options.Timeout) : -1.f;
	this->Active = this->HttpRequest->ProcessRequest();
}

void UHttpRequest::_Completed()
{
	this->Active = false;
	if (this->Completed.IsBound())
	{
		this->Completed.Broadcast(this->HttpResponse);
	}
	if (this->HttpResponse.IsFinished) 
	{
		RemoveFromRoot();
	}
}

void UHttpRequest::_Failed(EHttpRequestError Reason, FString Error)
{
	this->Active = false;
	this->HttpResponse.ErrorReason = Reason;
	this->HttpResponse.Error = Error;
	if (this->Failed.IsBound())
	{
		this->Failed.Broadcast(this->HttpResponse);
	}
	if (this->HttpResponse.IsFinished) 
	{
		RemoveFromRoot();
	}
}

void UHttpRequest::Tick(float DeltaTime)
{
	if (this->HttpRequest.IsValid())
	{
		if (TimeoutSeconds > 0.f)
		{
			TimeoutSeconds = FMath::Max<float>(TimeoutSeconds - DeltaTime, 0.f);
			if (TimeoutSeconds <= 0.f && this->HttpRequest->GetStatus() == EHttpRequestStatus::Processing)
			{
				this->HttpRequest->CancelRequest();
			}
		}
		this->HttpRequest->Tick(DeltaTime);
	}
}

void UHttpRequest::_Progress()
{
	if (this->Progress.IsBound())
	{
		this->Progress.Broadcast(this->HttpResponse);
	}
}