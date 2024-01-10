// Copyright (C) Developed by Neo Jin. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "TurboLinkGrpcMessage.generated.h"

#define DECLARE_JSON_FUNCTIONS() \
      TURBOLINKGRPC_API virtual FString ToJsonString(bool bPrettyMode) const override; \
      TURBOLINKGRPC_API virtual bool FromJsonString(const FString& JsonString) override;

#define DEFINE_JSON_FUNCTIONS(StructName, GrpcStructName) \
FString StructName::ToJsonString(bool bPrettyMode) const \
{ \
	GrpcStructName message; \
	TURBOLINK_TO_GRPC(this, &message); \
	std::string json_string; \
	::google::protobuf::util::JsonPrintOptions options; \
	options.add_whitespace=bPrettyMode; \
	if(::google::protobuf::util::MessageToJsonString(message,  &json_string, options).ok()) { \
		return FString(UTF8_TO_TCHAR(json_string.c_str())); \
	} \
	return FString(TEXT("")); \
} \
bool StructName::FromJsonString(const FString& JsonString) \
{ \
	GrpcStructName grpcMessage; \
	if(!::google::protobuf::util::JsonStringToMessage(TCHAR_TO_UTF8(*JsonString), &grpcMessage).ok()) return false; \
	GRPC_TO_TURBOLINK(&grpcMessage, this); \
	return true; \
}

#define DECLARE_SERIALIZE_FUNCTIONS() \
	TURBOLINKGRPC_API virtual TArray<uint8> ToBytes() const override; \
	TURBOLINKGRPC_API virtual bool FromBytes(const TArray<uint8>& Bytes) override;

#define DEFINE_SERIALIZE_FUNCTIONS(StructName, GrpcStructName) \
TArray<uint8> StructName::ToBytes() const \
{ \
	GrpcStructName message; \
	TURBOLINK_TO_GRPC(this, &message); \
	std::string output; \
	if(message.SerializeToString(&output)) { \
		FString InStr = FString(UTF8_TO_TCHAR(output.c_str())); \
		FTCHARToUTF8 Convert(*InStr); \
		int BytesLength = Convert.Length(); \
		uint8* messageBytes = static_cast<uint8*>(FMemory::Malloc(BytesLength)); \
		FMemory::Memcpy(messageBytes, (uint8*)TCHAR_TO_UTF8(InStr.GetCharArray().GetData()), BytesLength); \
		TArray<uint8> result; \
		for (int i = 0; i < BytesLength; i++) { \
			result.Add(messageBytes[i]); \
		} \
		FMemory::Free(messageBytes); \
		return result; \
	} \
	return {}; \
} \
bool StructName::FromBytes(const TArray<uint8>& Bytes) \
{ \
	GrpcStructName grpcMessage; \
	const std::string Str(reinterpret_cast<const char*>(Bytes.GetData()), Bytes.Num());	\
	if(!grpcMessage.ParseFromString(Str)) return false; \
	GRPC_TO_TURBOLINK(&grpcMessage, this); \
	return true; \
}

USTRUCT(BlueprintType)
struct FGrpcMessage
{
	GENERATED_BODY()
	virtual ~FGrpcMessage() = default;

	virtual FString ToJsonString(bool bPrettyMode) const { return FString(TEXT("{}")); }
	virtual bool FromJsonString(const FString& JsonString) { return false; }
	
	virtual TArray<uint8> ToBytes() const { return {}; }
	virtual bool FromBytes(const TArray<uint8>& Bytes) { return false; }
};


USTRUCT(BlueprintType, meta = (
	HasNativeMake = "TurboLinkGrpc.TurboLinkGrpcUtilities.MakeUInt64", 
	HasNativeBreak = "TurboLinkGrpc.TurboLinkGrpcUtilities.BreakUInt64"))
struct FUInt64
{
	GENERATED_BODY()

	uint64 Value;

	operator uint64() const { return Value; }

	FUInt64() { this->Value = 0; }
	FUInt64(const uint64& _Value) { this->Value = _Value; }
};

USTRUCT(BlueprintType, meta = (
	HasNativeMake = "TurboLinkGrpc.TurboLinkGrpcUtilities.MakeDouble64",
	HasNativeBreak = "TurboLinkGrpc.TurboLinkGrpcUtilities.BreakDouble64"))
struct FDouble64
{
	GENERATED_BODY()

	double Value;

	operator double() const { return Value; }

	FDouble64() { this->Value = 0.0; }
	FDouble64(const double& _Value) { this->Value = _Value; }
};

USTRUCT(BlueprintType, meta = (
	HasNativeMake = "TurboLinkGrpc.TurboLinkGrpcUtilities.MakeUInt32", 
	HasNativeBreak = "TurboLinkGrpc.TurboLinkGrpcUtilities.BreakUInt32"))
struct FUInt32
{
	GENERATED_BODY()

	uint32 Value;

	operator uint32() const { return Value; }

	FUInt32() { this->Value = 0; }
	FUInt32(const uint32& _Value) { this->Value = _Value; }
};

USTRUCT(BlueprintType)
struct FBytes
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = TurboLink)
	TArray<uint8> Value;

	FBytes() {  }
	FBytes(const uint8* _Value, int32 _Length) { Value.Append(_Value, _Length); }
};