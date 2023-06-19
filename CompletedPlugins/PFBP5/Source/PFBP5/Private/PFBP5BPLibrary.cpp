// Copyright Epic Games, Inc. All Rights Reserved.

#include "PFBP5BPLibrary.h"
#include "PFBP5.h"


#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <iterator>
#include <cstdint>

FString UPFBP5BPLibrary::encodeRequest(const FString& content, const FString& tts_type) {

	tutorial::Request vAudio;

	char* inContent = TCHAR_TO_UTF8(*content);
	char* inTts_type = TCHAR_TO_UTF8(*tts_type);

	vAudio.set_content(inContent);
	vAudio.set_tts_type(inTts_type);
	std::string* s = new std::string;
	vAudio.SerializeToString(s);

	return UTF8_TO_TCHAR(s->c_str());
}

FString UPFBP5BPLibrary::decodeRequest(const FString& r, FString& content, FString& tts_type) {

	tutorial::Request vAudio;
	char* strSet = TCHAR_TO_UTF8(*r);

	if (!vAudio.ParseFromString(strSet))
		return "Pre-Parsing failed";

	const std::string contentout = vAudio.content();
	const std::string ttsout = vAudio.tts_type();

	content = UTF8_TO_TCHAR(contentout.c_str());
	tts_type = UTF8_TO_TCHAR(ttsout.c_str());

	return "Pre-Parsing succeeded";
}


FString UPFBP5BPLibrary::Audio2Face(const TArray<uint8>& pbAudio, FString& content, float& emotion, TArray<float>& bs_value, TArray<FName>& bs_key, TArray<uint8>& wav_data) {
	tutorial::Response vAudio;

	const char* strSet = (char*)pbAudio.GetData();
	int strLen = pbAudio.Num();

	if (!vAudio.ParseFromArray(strSet, strLen))
		return "Parsing failed";

	content = UTF8_TO_TCHAR(vAudio.content().c_str());
	emotion = vAudio.emotion();

	for (int i = 0; i < vAudio.bs_value().size(); i++) {
		bs_value.Add(vAudio.bs_value(i));
	}

	for (int i = 0; i < vAudio.bs_key().size(); i++) {
		bs_key.Add(UTF8_TO_TCHAR(vAudio.bs_key(i).c_str()));
	}

	wav_data = TArray<uint8>((uint8*)(vAudio.wav_data().c_str()), vAudio.wav_data().length());

	return "Parsing succeeded";
}

UPFBP5BPLibrary* UPFBP5BPLibrary::requestPost(const FString& url, const FString& data, const FString& content, const FString& tts_type) {

	UPFBP5BPLibrary* up = NewObject<UPFBP5BPLibrary>();
	up->sendPOSTRequest(url, data, content, tts_type);

	return up;

}

void UPFBP5BPLibrary::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {


	if (bWasSuccessful && Response.IsValid())
	{
		TArray<uint8> arrayData = Response->GetContent();

		OnSuccess.Broadcast(
			Response->GetResponseCode(),
			arrayData

		);
	}
	else
	{
		TArray<uint8> arrayData;
		OnFail.Broadcast(-1, arrayData);
	}

	RemoveFromRoot();
}

FString UPFBP5BPLibrary::getResult(UPFBP5BPLibrary* p) {
	return p->result;
}

void UPFBP5BPLibrary::sendPOSTRequest(const FString& url, const FString& data, const FString& content, const FString& tts_type) {
	AddToRoot();

	tutorial::Request vAudio;

	char* inContent = TCHAR_TO_UTF8(*content);
	char* inTts_type = TCHAR_TO_UTF8(*tts_type);

	vAudio.set_content(inContent);
	vAudio.set_tts_type(inTts_type);
	std::string* s = new std::string;
	vAudio.SerializeToString(s);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPFBP5BPLibrary::OnResponseReceived);
	Request->SetURL(url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/data"));
	Request->SetContentAsString(UTF8_TO_TCHAR(s->c_str()));
	Request->ProcessRequest();


}
