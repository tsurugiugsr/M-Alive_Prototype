// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "M_Alive_Prototype/M_Alive_PrototypeGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeM_Alive_PrototypeGameModeBase() {}
// Cross Module References
	M_ALIVE_PROTOTYPE_API UClass* Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_NoRegister();
	M_ALIVE_PROTOTYPE_API UClass* Z_Construct_UClass_AM_Alive_PrototypeGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_M_Alive_Prototype();
// End Cross Module References
	void AM_Alive_PrototypeGameModeBase::StaticRegisterNativesAM_Alive_PrototypeGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AM_Alive_PrototypeGameModeBase);
	UClass* Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_NoRegister()
	{
		return AM_Alive_PrototypeGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_M_Alive_Prototype,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "M_Alive_PrototypeGameModeBase.h" },
		{ "ModuleRelativePath", "M_Alive_PrototypeGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AM_Alive_PrototypeGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::ClassParams = {
		&AM_Alive_PrototypeGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AM_Alive_PrototypeGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AM_Alive_PrototypeGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AM_Alive_PrototypeGameModeBase.OuterSingleton, Z_Construct_UClass_AM_Alive_PrototypeGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AM_Alive_PrototypeGameModeBase.OuterSingleton;
	}
	template<> M_ALIVE_PROTOTYPE_API UClass* StaticClass<AM_Alive_PrototypeGameModeBase>()
	{
		return AM_Alive_PrototypeGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AM_Alive_PrototypeGameModeBase);
	struct Z_CompiledInDeferFile_FID_M_Alive_Prototype_Source_M_Alive_Prototype_M_Alive_PrototypeGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_M_Alive_Prototype_Source_M_Alive_Prototype_M_Alive_PrototypeGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AM_Alive_PrototypeGameModeBase, AM_Alive_PrototypeGameModeBase::StaticClass, TEXT("AM_Alive_PrototypeGameModeBase"), &Z_Registration_Info_UClass_AM_Alive_PrototypeGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AM_Alive_PrototypeGameModeBase), 2423551680U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_M_Alive_Prototype_Source_M_Alive_Prototype_M_Alive_PrototypeGameModeBase_h_4250474918(TEXT("/Script/M_Alive_Prototype"),
		Z_CompiledInDeferFile_FID_M_Alive_Prototype_Source_M_Alive_Prototype_M_Alive_PrototypeGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_M_Alive_Prototype_Source_M_Alive_Prototype_M_Alive_PrototypeGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
