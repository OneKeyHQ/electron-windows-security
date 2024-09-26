// Copyright (c) The NodeRT Contributors
// All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the ""License""); you may
// not use this file except in compliance with the License. You may obtain a
// copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
// OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
// IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
//
// See the Apache Version 2.0 License for specific language governing permissions
// and limitations under the License.

// TODO: Verify that this is is still needed..
#define NTDDI_VERSION 0x06010000

#include <v8.h>
#include "nan.h"
#include <string>
#include <ppltasks.h>
#include "CollectionsConverter.h"
#include "CollectionsWrap.h"
#include "node-async.h"
#include "NodeRtUtils.h"
#include "OpaqueWrapper.h"
#include "WrapperBase.h"

#using <Windows.WinMD>

// this undefs fixes the issues of compiling Windows.Data.Json, Windows.Storag.FileProperties, and Windows.Stroage.Search
// Some of the node header files brings windows definitions with the same names as some of the WinRT methods
#undef DocumentProperties
#undef GetObject
#undef CreateEvent
#undef FindText
#undef SendMessage

const char* REGISTRATION_TOKEN_MAP_PROPERTY_NAME = "__registrationTokenMap__";

using v8::Array;
using v8::String;
using v8::Value;
using v8::Boolean;
using v8::Integer;
using v8::FunctionTemplate;
using v8::Object;
using v8::Local;
using v8::Function;
using v8::Date;
using v8::Number;
using v8::PropertyAttribute;
using v8::Primitive;
using Nan::HandleScope;
using Nan::Persistent;
using Nan::Undefined;
using Nan::True;
using Nan::False;
using Nan::Null;
using Nan::MaybeLocal;
using Nan::EscapableHandleScope;
using Nan::HandleScope;
using Nan::TryCatch;
using namespace concurrency;

namespace NodeRT { namespace Windows { namespace Security { namespace Credentials { namespace UI { 
  v8::Local<v8::Value> WrapCredentialPicker(::Windows::Security::Credentials::UI::CredentialPicker^ wintRtInstance);
  ::Windows::Security::Credentials::UI::CredentialPicker^ UnwrapCredentialPicker(Local<Value> value);
  
  v8::Local<v8::Value> WrapCredentialPickerOptions(::Windows::Security::Credentials::UI::CredentialPickerOptions^ wintRtInstance);
  ::Windows::Security::Credentials::UI::CredentialPickerOptions^ UnwrapCredentialPickerOptions(Local<Value> value);
  
  v8::Local<v8::Value> WrapCredentialPickerResults(::Windows::Security::Credentials::UI::CredentialPickerResults^ wintRtInstance);
  ::Windows::Security::Credentials::UI::CredentialPickerResults^ UnwrapCredentialPickerResults(Local<Value> value);
  
  v8::Local<v8::Value> WrapUserConsentVerifier(::Windows::Security::Credentials::UI::UserConsentVerifier^ wintRtInstance);
  ::Windows::Security::Credentials::UI::UserConsentVerifier^ UnwrapUserConsentVerifier(Local<Value> value);
  



  static void InitAuthenticationProtocolEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("AuthenticationProtocol").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("basic").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::Basic)));
    Nan::Set(enumObject, Nan::New<String>("digest").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::Digest)));
    Nan::Set(enumObject, Nan::New<String>("ntlm").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::Ntlm)));
    Nan::Set(enumObject, Nan::New<String>("kerberos").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::Kerberos)));
    Nan::Set(enumObject, Nan::New<String>("negotiate").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::Negotiate)));
    Nan::Set(enumObject, Nan::New<String>("credSsp").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::CredSsp)));
    Nan::Set(enumObject, Nan::New<String>("custom").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::AuthenticationProtocol::Custom)));
  }

  static void InitCredentialSaveOptionEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("CredentialSaveOption").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("unselected").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::CredentialSaveOption::Unselected)));
    Nan::Set(enumObject, Nan::New<String>("selected").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::CredentialSaveOption::Selected)));
    Nan::Set(enumObject, Nan::New<String>("hidden").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::CredentialSaveOption::Hidden)));
  }

  static void InitUserConsentVerificationResultEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("UserConsentVerificationResult").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("verified").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::Verified)));
    Nan::Set(enumObject, Nan::New<String>("deviceNotPresent").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::DeviceNotPresent)));
    Nan::Set(enumObject, Nan::New<String>("notConfiguredForUser").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::NotConfiguredForUser)));
    Nan::Set(enumObject, Nan::New<String>("disabledByPolicy").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::DisabledByPolicy)));
    Nan::Set(enumObject, Nan::New<String>("deviceBusy").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::DeviceBusy)));
    Nan::Set(enumObject, Nan::New<String>("retriesExhausted").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::RetriesExhausted)));
    Nan::Set(enumObject, Nan::New<String>("canceled").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerificationResult::Canceled)));
  }

  static void InitUserConsentVerifierAvailabilityEnum(const Local<Object> exports) {
    HandleScope scope;

    Local<Object> enumObject = Nan::New<Object>();

    Nan::Set(exports, Nan::New<String>("UserConsentVerifierAvailability").ToLocalChecked(), enumObject);
    Nan::Set(enumObject, Nan::New<String>("available").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerifierAvailability::Available)));
    Nan::Set(enumObject, Nan::New<String>("deviceNotPresent").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerifierAvailability::DeviceNotPresent)));
    Nan::Set(enumObject, Nan::New<String>("notConfiguredForUser").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerifierAvailability::NotConfiguredForUser)));
    Nan::Set(enumObject, Nan::New<String>("disabledByPolicy").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerifierAvailability::DisabledByPolicy)));
    Nan::Set(enumObject, Nan::New<String>("deviceBusy").ToLocalChecked(), Nan::New<Integer>(static_cast<int>(::Windows::Security::Credentials::UI::UserConsentVerifierAvailability::DeviceBusy)));
  }



  class CredentialPicker : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("CredentialPicker").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);

        Local<Function> func;
        Local<FunctionTemplate> funcTemplate;





        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);

        func = Nan::GetFunction(Nan::New<FunctionTemplate>(PickAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("pickAsync").ToLocalChecked(), func);


        Nan::Set(exports, Nan::New<String>("CredentialPicker").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      CredentialPicker(::Windows::Security::Credentials::UI::CredentialPicker^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::Security::Credentials::UI::CredentialPicker^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPicker^>(info[0])) {
        try {
          winRtInstance = (::Windows::Security::Credentials::UI::CredentialPicker^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      CredentialPicker *wrapperInstance = new CredentialPicker(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPicker^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::Security::Credentials::UI::CredentialPicker^ winRtInstance;
      try {
        winRtInstance = (::Windows::Security::Credentials::UI::CredentialPicker^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapCredentialPicker(winRtInstance));
    }




    static void PickAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::Security::Credentials::UI::CredentialPickerResults^>^ op;


      if (info.Length() == 2
          && NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info[0]))
      {
        try
        {
          ::Windows::Security::Credentials::UI::CredentialPickerOptions^ arg0 = UnwrapCredentialPickerOptions(info[0]);
            
          op = ::Windows::Security::Credentials::UI::CredentialPicker::PickAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
      else if (info.Length() == 3
          && info[0]->IsString()
          && info[1]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          Platform::String^ arg1 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[1])));
            
          op = ::Windows::Security::Credentials::UI::CredentialPicker::PickAsync(arg0,arg1);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
      else if (info.Length() == 4
          && info[0]->IsString()
          && info[1]->IsString()
          && info[2]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
          Platform::String^ arg1 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[1])));
          Platform::String^ arg2 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[2])));
            
          op = ::Windows::Security::Credentials::UI::CredentialPicker::PickAsync(arg0,arg1,arg2);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::Security::Credentials::UI::CredentialPickerResults^> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = WrapCredentialPickerResults(result);
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }




    private:
      ::Windows::Security::Credentials::UI::CredentialPicker^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapCredentialPicker(::Windows::Security::Credentials::UI::CredentialPicker^ wintRtInstance);
      friend ::Windows::Security::Credentials::UI::CredentialPicker^ UnwrapCredentialPicker(Local<Value> value);
  };

  Persistent<FunctionTemplate> CredentialPicker::s_constructorTemplate;

  v8::Local<v8::Value> WrapCredentialPicker(::Windows::Security::Credentials::UI::CredentialPicker^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(CredentialPicker::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::Security::Credentials::UI::CredentialPicker^ UnwrapCredentialPicker(Local<Value> value) {
     return CredentialPicker::Unwrap<CredentialPicker>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitCredentialPicker(Local<Object> exports) {
    CredentialPicker::Init(exports);
  }

  class CredentialPickerOptions : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("CredentialPickerOptions").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("targetName").ToLocalChecked(), TargetNameGetter, TargetNameSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("previousCredential").ToLocalChecked(), PreviousCredentialGetter, PreviousCredentialSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("message").ToLocalChecked(), MessageGetter, MessageSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("errorCode").ToLocalChecked(), ErrorCodeGetter, ErrorCodeSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("customAuthenticationProtocol").ToLocalChecked(), CustomAuthenticationProtocolGetter, CustomAuthenticationProtocolSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credentialSaveOption").ToLocalChecked(), CredentialSaveOptionGetter, CredentialSaveOptionSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("caption").ToLocalChecked(), CaptionGetter, CaptionSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("callerSavesCredential").ToLocalChecked(), CallerSavesCredentialGetter, CallerSavesCredentialSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("authenticationProtocol").ToLocalChecked(), AuthenticationProtocolGetter, AuthenticationProtocolSetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("alwaysDisplayDialog").ToLocalChecked(), AlwaysDisplayDialogGetter, AlwaysDisplayDialogSetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("CredentialPickerOptions").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      CredentialPickerOptions(::Windows::Security::Credentials::UI::CredentialPickerOptions^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::Security::Credentials::UI::CredentialPickerOptions^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info[0])) {
        try {
          winRtInstance = (::Windows::Security::Credentials::UI::CredentialPickerOptions^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
      else if (info.Length() == 0)
      {
        try {
          winRtInstance = ref new ::Windows::Security::Credentials::UI::CredentialPickerOptions();
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      CredentialPickerOptions *wrapperInstance = new CredentialPickerOptions(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::Security::Credentials::UI::CredentialPickerOptions^ winRtInstance;
      try {
        winRtInstance = (::Windows::Security::Credentials::UI::CredentialPickerOptions^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapCredentialPickerOptions(winRtInstance));
    }





    static void TargetNameGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->TargetName;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void TargetNameSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsString()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        Platform::String^ winRtValue = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));

        wrapper->_instance->TargetName = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void PreviousCredentialGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        ::Windows::Storage::Streams::IBuffer^ result = wrapper->_instance->PreviousCredential;
        info.GetReturnValue().Set(NodeRT::Utils::CreateExternalWinRTObject("Windows.Storage.Streams", "IBuffer", result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void PreviousCredentialSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Storage::Streams::IBuffer^>(value)) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        ::Windows::Storage::Streams::IBuffer^ winRtValue = dynamic_cast<::Windows::Storage::Streams::IBuffer^>(NodeRT::Utils::GetObjectInstance(value));

        wrapper->_instance->PreviousCredential = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void MessageGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Message;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void MessageSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsString()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        Platform::String^ winRtValue = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));

        wrapper->_instance->Message = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void ErrorCodeGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        unsigned int result = wrapper->_instance->ErrorCode;
        info.GetReturnValue().Set(Nan::New<Integer>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ErrorCodeSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsUint32()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        unsigned int winRtValue = static_cast<unsigned int>(Nan::To<uint32_t>(value).FromMaybe(0));

        wrapper->_instance->ErrorCode = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void CustomAuthenticationProtocolGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CustomAuthenticationProtocol;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CustomAuthenticationProtocolSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsString()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        Platform::String^ winRtValue = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));

        wrapper->_instance->CustomAuthenticationProtocol = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void CredentialSaveOptionGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        ::Windows::Security::Credentials::UI::CredentialSaveOption result = wrapper->_instance->CredentialSaveOption;
        info.GetReturnValue().Set(Nan::New<Integer>(static_cast<int>(result)));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CredentialSaveOptionSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsInt32()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        ::Windows::Security::Credentials::UI::CredentialSaveOption winRtValue = static_cast<::Windows::Security::Credentials::UI::CredentialSaveOption>(Nan::To<int32_t>(value).FromMaybe(0));

        wrapper->_instance->CredentialSaveOption = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void CaptionGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->Caption;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CaptionSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsString()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        Platform::String^ winRtValue = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), value)));

        wrapper->_instance->Caption = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void CallerSavesCredentialGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        bool result = wrapper->_instance->CallerSavesCredential;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CallerSavesCredentialSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsBoolean()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        bool winRtValue = Nan::To<bool>(value).FromMaybe(false);

        wrapper->_instance->CallerSavesCredential = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void AuthenticationProtocolGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        ::Windows::Security::Credentials::UI::AuthenticationProtocol result = wrapper->_instance->AuthenticationProtocol;
        info.GetReturnValue().Set(Nan::New<Integer>(static_cast<int>(result)));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void AuthenticationProtocolSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsInt32()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        ::Windows::Security::Credentials::UI::AuthenticationProtocol winRtValue = static_cast<::Windows::Security::Credentials::UI::AuthenticationProtocol>(Nan::To<int32_t>(value).FromMaybe(0));

        wrapper->_instance->AuthenticationProtocol = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      
    static void AlwaysDisplayDialogGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try  {
        bool result = wrapper->_instance->AlwaysDisplayDialog;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void AlwaysDisplayDialogSetter(Local<String> property, Local<Value> value, const Nan::PropertyCallbackInfo<void> &info) {
      HandleScope scope;

      if (!value->IsBoolean()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Value to set is of unexpected type")));
        return;
      }

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerOptions^>(info.This())) {
        return;
      }

      CredentialPickerOptions *wrapper = CredentialPickerOptions::Unwrap<CredentialPickerOptions>(info.This());

      try {

        bool winRtValue = Nan::To<bool>(value).FromMaybe(false);

        wrapper->_instance->AlwaysDisplayDialog = winRtValue;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
      }
    }
      


    private:
      ::Windows::Security::Credentials::UI::CredentialPickerOptions^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapCredentialPickerOptions(::Windows::Security::Credentials::UI::CredentialPickerOptions^ wintRtInstance);
      friend ::Windows::Security::Credentials::UI::CredentialPickerOptions^ UnwrapCredentialPickerOptions(Local<Value> value);
  };

  Persistent<FunctionTemplate> CredentialPickerOptions::s_constructorTemplate;

  v8::Local<v8::Value> WrapCredentialPickerOptions(::Windows::Security::Credentials::UI::CredentialPickerOptions^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(CredentialPickerOptions::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::Security::Credentials::UI::CredentialPickerOptions^ UnwrapCredentialPickerOptions(Local<Value> value) {
     return CredentialPickerOptions::Unwrap<CredentialPickerOptions>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitCredentialPickerOptions(Local<Object> exports) {
    CredentialPickerOptions::Init(exports);
  }

  class CredentialPickerResults : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("CredentialPickerResults").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);





          
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credential").ToLocalChecked(), CredentialGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credentialDomainName").ToLocalChecked(), CredentialDomainNameGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credentialPassword").ToLocalChecked(), CredentialPasswordGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credentialSaveOption").ToLocalChecked(), CredentialSaveOptionGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credentialSaved").ToLocalChecked(), CredentialSavedGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("credentialUserName").ToLocalChecked(), CredentialUserNameGetter);
            Nan::SetAccessor(localRef->PrototypeTemplate(), Nan::New<String>("errorCode").ToLocalChecked(), ErrorCodeGetter);

        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);



        Nan::Set(exports, Nan::New<String>("CredentialPickerResults").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      CredentialPickerResults(::Windows::Security::Credentials::UI::CredentialPickerResults^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::Security::Credentials::UI::CredentialPickerResults^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info[0])) {
        try {
          winRtInstance = (::Windows::Security::Credentials::UI::CredentialPickerResults^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      CredentialPickerResults *wrapperInstance = new CredentialPickerResults(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::Security::Credentials::UI::CredentialPickerResults^ winRtInstance;
      try {
        winRtInstance = (::Windows::Security::Credentials::UI::CredentialPickerResults^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapCredentialPickerResults(winRtInstance));
    }





    static void CredentialGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        ::Windows::Storage::Streams::IBuffer^ result = wrapper->_instance->Credential;
        info.GetReturnValue().Set(NodeRT::Utils::CreateExternalWinRTObject("Windows.Storage.Streams", "IBuffer", result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CredentialDomainNameGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CredentialDomainName;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CredentialPasswordGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CredentialPassword;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CredentialSaveOptionGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        ::Windows::Security::Credentials::UI::CredentialSaveOption result = wrapper->_instance->CredentialSaveOption;
        info.GetReturnValue().Set(Nan::New<Integer>(static_cast<int>(result)));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CredentialSavedGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        bool result = wrapper->_instance->CredentialSaved;
        info.GetReturnValue().Set(Nan::New<Boolean>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void CredentialUserNameGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        Platform::String^ result = wrapper->_instance->CredentialUserName;
        info.GetReturnValue().Set(NodeRT::Utils::NewString(result->Data()));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      
    static void ErrorCodeGetter(Local<String> property, const Nan::PropertyCallbackInfo<v8::Value> &info) {
      HandleScope scope;

      if (!NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::CredentialPickerResults^>(info.This())) {
        return;
      }

      CredentialPickerResults *wrapper = CredentialPickerResults::Unwrap<CredentialPickerResults>(info.This());

      try  {
        unsigned int result = wrapper->_instance->ErrorCode;
        info.GetReturnValue().Set(Nan::New<Integer>(result));
        return;
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }
    }
      


    private:
      ::Windows::Security::Credentials::UI::CredentialPickerResults^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapCredentialPickerResults(::Windows::Security::Credentials::UI::CredentialPickerResults^ wintRtInstance);
      friend ::Windows::Security::Credentials::UI::CredentialPickerResults^ UnwrapCredentialPickerResults(Local<Value> value);
  };

  Persistent<FunctionTemplate> CredentialPickerResults::s_constructorTemplate;

  v8::Local<v8::Value> WrapCredentialPickerResults(::Windows::Security::Credentials::UI::CredentialPickerResults^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(CredentialPickerResults::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::Security::Credentials::UI::CredentialPickerResults^ UnwrapCredentialPickerResults(Local<Value> value) {
     return CredentialPickerResults::Unwrap<CredentialPickerResults>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitCredentialPickerResults(Local<Object> exports) {
    CredentialPickerResults::Init(exports);
  }

  class UserConsentVerifier : public WrapperBase {
    public:
      
      static void Init(const Local<Object> exports) {
        HandleScope scope;

        Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(New);
        s_constructorTemplate.Reset(localRef);
        localRef->SetClassName(Nan::New<String>("UserConsentVerifier").ToLocalChecked());
        localRef->InstanceTemplate()->SetInternalFieldCount(1);

        Local<Function> func;
        Local<FunctionTemplate> funcTemplate;





        Local<Object> constructor = Nan::To<Object>(Nan::GetFunction(localRef).ToLocalChecked()).ToLocalChecked();
        Nan::SetMethod(constructor, "castFrom", CastFrom);

        func = Nan::GetFunction(Nan::New<FunctionTemplate>(CheckAvailabilityAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("checkAvailabilityAsync").ToLocalChecked(), func);
        func = Nan::GetFunction(Nan::New<FunctionTemplate>(RequestVerificationAsync)).ToLocalChecked();
        Nan::Set(constructor, Nan::New<String>("requestVerificationAsync").ToLocalChecked(), func);


        Nan::Set(exports, Nan::New<String>("UserConsentVerifier").ToLocalChecked(), constructor);
      }

      virtual ::Platform::Object^ GetObjectInstance() const override {
        return _instance;
      }

    private:

      UserConsentVerifier(::Windows::Security::Credentials::UI::UserConsentVerifier^ instance) {
        _instance = instance;
      }

      
    static void New(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(s_constructorTemplate);

      // in case the constructor was called without the new operator
      if (!localRef->HasInstance(info.This())) {
        if (info.Length() > 0) {
          std::unique_ptr<Local<Value> []> constructorArgs(new Local<Value>[info.Length()]);

          Local<Value> *argsPtr = constructorArgs.get();
          for (int i = 0; i < info.Length(); i++) {
            argsPtr[i] = info[i];
          }

          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), constructorArgs.get());
          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        } else {
          MaybeLocal<Object> res = Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(), info.Length(), nullptr);

          if (res.IsEmpty()) {
            return;
          }

          info.GetReturnValue().Set(res.ToLocalChecked());
          return;
        }
      }

      ::Windows::Security::Credentials::UI::UserConsentVerifier^ winRtInstance;


      if (info.Length() == 1 && OpaqueWrapper::IsOpaqueWrapper(info[0]) &&
        NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::UserConsentVerifier^>(info[0])) {
        try {
          winRtInstance = (::Windows::Security::Credentials::UI::UserConsentVerifier^) NodeRT::Utils::GetObjectInstance(info[0]);
        } catch (Platform::Exception ^exception) {
          NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
          return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no suitable constructor found")));
        return;
      }

      NodeRT::Utils::SetHiddenValue(info.This(), Nan::New<String>("__winRtInstance__").ToLocalChecked(), True());

      UserConsentVerifier *wrapperInstance = new UserConsentVerifier(winRtInstance);
      wrapperInstance->Wrap(info.This());

      info.GetReturnValue().Set(info.This());
    }


      
    static void CastFrom(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;
      if (info.Length() < 1 || !NodeRT::Utils::IsWinRtWrapperOf<::Windows::Security::Credentials::UI::UserConsentVerifier^>(info[0])) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Invalid arguments, no object provided, or given object could not be casted to requested type")));
        return;
      }

      ::Windows::Security::Credentials::UI::UserConsentVerifier^ winRtInstance;
      try {
        winRtInstance = (::Windows::Security::Credentials::UI::UserConsentVerifier^) NodeRT::Utils::GetObjectInstance(info[0]);
      } catch (Platform::Exception ^exception) {
        NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
        return;
      }

      info.GetReturnValue().Set(WrapUserConsentVerifier(winRtInstance));
    }




    static void CheckAvailabilityAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::Security::Credentials::UI::UserConsentVerifierAvailability>^ op;


      if (info.Length() == 1)
      {
        try
        {
          op = ::Windows::Security::Credentials::UI::UserConsentVerifier::CheckAvailabilityAsync();
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::Security::Credentials::UI::UserConsentVerifierAvailability> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = Nan::New<Integer>(static_cast<int>(result));
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }

    static void RequestVerificationAsync(Nan::NAN_METHOD_ARGS_TYPE info) {
      HandleScope scope;

      if (info.Length() == 0 || !info[info.Length() -1]->IsFunction()) {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: No callback was given")));
        return;
      }

      ::Windows::Foundation::IAsyncOperation<::Windows::Security::Credentials::UI::UserConsentVerificationResult>^ op;


      if (info.Length() == 2
          && info[0]->IsString())
      {
        try
        {
          Platform::String^ arg0 = ref new Platform::String(NodeRT::Utils::StringToWchar(v8::String::Value(v8::Isolate::GetCurrent(), info[0])));
            
          op = ::Windows::Security::Credentials::UI::UserConsentVerifier::RequestVerificationAsync(arg0);
        } catch (Platform::Exception ^exception) {
            NodeRT::Utils::ThrowWinRtExceptionInJs(exception);
            return;
        }
      }
 else {
        Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"Bad arguments: no suitable overload found")));
        return;
      }

      auto opTask = create_task(op);
      uv_async_t* asyncToken = NodeUtils::Async::GetAsyncToken(info[info.Length() -1].As<Function>());

      opTask.then( [asyncToken] (task<::Windows::Security::Credentials::UI::UserConsentVerificationResult> t)
      {
        try {
          auto result = t.get();
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [result](NodeUtils::InvokeCallbackDelegate invokeCallback) {


            Local<Value> error;
            Local<Value> arg1;
            {
              TryCatch tryCatch;
              arg1 = Nan::New<Integer>(static_cast<int>(result));
              if (tryCatch.HasCaught())
              {
                error = Nan::To<Object>(tryCatch.Exception()).ToLocalChecked();
              }
              else
              {
                error = Undefined();
              }
              if (arg1.IsEmpty()) arg1 = Undefined();
            }
            Local<Value> args[] = {error, arg1};


            invokeCallback(_countof(args), args);
          });
        }
        catch (Platform::Exception^ exception)
        {
          NodeUtils::Async::RunCallbackOnMain(asyncToken, [exception](NodeUtils::InvokeCallbackDelegate invokeCallback) {

            Local<Value> error = NodeRT::Utils::WinRtExceptionToJsError(exception);

            Local<Value> args[] = {error};
            invokeCallback(_countof(args), args);
          });
        }
      });
    }




    private:
      ::Windows::Security::Credentials::UI::UserConsentVerifier^ _instance;
      static Persistent<FunctionTemplate> s_constructorTemplate;

      friend v8::Local<v8::Value> WrapUserConsentVerifier(::Windows::Security::Credentials::UI::UserConsentVerifier^ wintRtInstance);
      friend ::Windows::Security::Credentials::UI::UserConsentVerifier^ UnwrapUserConsentVerifier(Local<Value> value);
  };

  Persistent<FunctionTemplate> UserConsentVerifier::s_constructorTemplate;

  v8::Local<v8::Value> WrapUserConsentVerifier(::Windows::Security::Credentials::UI::UserConsentVerifier^ winRtInstance) {
    EscapableHandleScope scope;

    if (winRtInstance == nullptr) {
      return scope.Escape(Undefined());
    }

    Local<Value> opaqueWrapper = CreateOpaqueWrapper(winRtInstance);
    Local<Value> args[] = {opaqueWrapper};
    Local<FunctionTemplate> localRef = Nan::New<FunctionTemplate>(UserConsentVerifier::s_constructorTemplate);
    return scope.Escape(Nan::NewInstance(Nan::GetFunction(localRef).ToLocalChecked(),_countof(args), args).ToLocalChecked());
  }

  ::Windows::Security::Credentials::UI::UserConsentVerifier^ UnwrapUserConsentVerifier(Local<Value> value) {
     return UserConsentVerifier::Unwrap<UserConsentVerifier>(Nan::To<Object>(value).ToLocalChecked())->_instance;
  }

  void InitUserConsentVerifier(Local<Object> exports) {
    UserConsentVerifier::Init(exports);
  }


} } } } } 

NAN_MODULE_INIT(init) {
  // We ignore failures for now since it probably means that
  // the initialization already happened for STA, and that's cool

  CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  /*
  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
    Nan::ThrowError(Nan::Error(NodeRT::Utils::NewString(L"error in CoInitializeEx()")));
    return;
  }
  */

      NodeRT::Windows::Security::Credentials::UI::InitAuthenticationProtocolEnum(target);
      NodeRT::Windows::Security::Credentials::UI::InitCredentialSaveOptionEnum(target);
      NodeRT::Windows::Security::Credentials::UI::InitUserConsentVerificationResultEnum(target);
      NodeRT::Windows::Security::Credentials::UI::InitUserConsentVerifierAvailabilityEnum(target);
      NodeRT::Windows::Security::Credentials::UI::InitCredentialPicker(target);
      NodeRT::Windows::Security::Credentials::UI::InitCredentialPickerOptions(target);
      NodeRT::Windows::Security::Credentials::UI::InitCredentialPickerResults(target);
      NodeRT::Windows::Security::Credentials::UI::InitUserConsentVerifier(target);


  NodeRT::Utils::RegisterNameSpace("Windows.Security.Credentials.UI", target);
}



NODE_MODULE(binding, init)
