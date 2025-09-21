#pragma once
#include "CoreMinimal.h"


// #define _ToStrImpl(x) #x
// #define TO_STR(x) _ToStrImpl(x)
// 
// #if defined(PLATFORM_WINDOWS) && defined(_MSC_VER)
// #if PLATFORM_WINDOWS
// #define _Output(msg) __pragma(message(__FILE__"("TO_STR(__LINE__)"): " msg))
// 
// #define ENABLE_OPTIMIZE \
// 	__pragma(optimize("", on)) \
// 	_Output("Optimize Enable")
// #define DISABLE_OPTIMIZE \
// 	__pragma(optimize("",  off)) \
// 	_Output("Optimize Disable")
// #endif	// #if PLATFORM_WINDOWS
// #endif	// #ifdef PLATFORM_WINDOWS
// 
// #ifndef ENABLE_OPTIMIZE
// #define ENABLE_OPTIMIZE
// #endif	// #ifndef ENABLE_OPTIMIZE
// 
// #ifndef DISABLE_OPTIMIZE
// #define DISABLE_OPTIMIZE
// #endif	// #ifndef DISABLE_OPTIMIZE
// 
// #if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
// #define DEVELOPMENT_MODE 1
// #else
// #define DEVELOPMENT_MODE 0
// #endif
// 
// #define CHARACTERVCULLING_TEST
// 
// //Alpha 0.3 version
// #define JOYSTICK_MODE 1
// 
// #define MFR_USINGPSO_LOAD 1
// 
// #define SAFE_DELETE(p) { if(p) { delete (p); (p)=nullptr; } }
// 
// //////////////////////////////////////////////////////////////////////////
// // 로그 카테고리 이 안에 선언해주세요.
// // 기본 로그 카테고리
// #if DEVELOPMENT_MODE
// DECLARE_LOG_CATEGORY_EXTERN(MONSTERLOG, Log, All);
// #else
// DECLARE_LOG_CATEGORY_EXTERN(MONSTERLOG, Log, Display);
// #endif
// 
// DECLARE_LOG_CATEGORY_EXTERN(MON_CDT_FNC_PRG, Log, All);
// 
// // 기획에서 원하는 로그 출력
// DECLARE_LOG_CATEGORY_EXTERN(PS_ZONE, Log, All);
// DECLARE_LOG_CATEGORY_EXTERN(PS_BATTLE, Log, All);
// DECLARE_LOG_CATEGORY_EXTERN(PS_OBJECT, Log, All);
// DECLARE_LOG_CATEGORY_EXTERN(PS_MISSION, Log, All);
// 
// //이동 관련 Tracking
// DECLARE_LOG_CATEGORY_EXTERN(PS_MOVE, Log, All);
// 
// //#endif
// //////////////////////////////////////////////////////////////////////////
// 
// #if WITH_EDITOR
// #define _PS_LOG_WITH_FUNC_FILE_LINE(CategoryName, Verbosity, Format, ...) \
// UE_LOG(CategoryName, Verbosity, TEXT("%s [Func: %s] [File: %s] [Line: %d]"), *FString::Printf(Format, ##__VA_ARGS__), *FString(__FUNCTION__), TEXT(__FILE__), __LINE__)
// #else
// #define _PS_LOG_WITH_FUNC_FILE_LINE(CategoryName, Verbosity, Format, ...) UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)
// #endif
// 
// #if WITH_EDITOR
//     #define USE_BREAD_CRUMB 0
// #else
//     #define USE_BREAD_CRUMB 1
// #endif
// 
// #if USE_BREAD_CRUMB
// namespace PSLog
// {
// 	extern ELogVerbosity::Type BreadCrumbVerbosity;
// 	extern void LeaveBreadCrumb(const FString& Str);
// }
// #define PS_LOG(CategoryName, Verbosity, Format, ...) \
// { \
// 	if (ELogVerbosity::Verbosity <= PSLog::BreadCrumbVerbosity) \
// 	{ \
// 		PSLog::LeaveBreadCrumb(FString::Printf(Format, ##__VA_ARGS__)); \
// 	} \
// 	_PS_LOG_WITH_FUNC_FILE_LINE(CategoryName, Verbosity, Format, ##__VA_ARGS__); \
// }
// #else
// #define PS_LOG(CategoryName, Verbosity, Format, ...) _PS_LOG_WITH_FUNC_FILE_LINE(CategoryName, Verbosity, Format, ##__VA_ARGS__)
// #endif
// 
// #define PS_UENUM_NAME(UEnumValue, UEnumType) FName UEnumValue##Name; UEnum* U##UEnumValue##Ptr = FindObject<UEnum>(ANY_PACKAGE, TEXT(#UEnumType), true); \
// if (U##UEnumValue##Ptr) \
//     UEnumValue##Name = FName(*U##UEnumValue##Ptr->GetNameStringByValue(static_cast<int64>(UEnumValue))); \
// else \
//     UEnumValue##Name = FName(*FString::FromInt((int32)UEnumValue));