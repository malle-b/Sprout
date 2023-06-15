// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RHelperDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/home/malin/hades/software/src/RHelper/include/HistogramHelper.h"
#include "/home/malin/hades/software/src/RHelper/include/RHtree.h"
#include "/home/malin/hades/software/src/RHelper/include/RHfit.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_RHtree(void *p = nullptr);
   static void *newArray_RHtree(Long_t size, void *p);
   static void delete_RHtree(void *p);
   static void deleteArray_RHtree(void *p);
   static void destruct_RHtree(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RHtree*)
   {
      ::RHtree *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RHtree >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("RHtree", ::RHtree::Class_Version(), "include/RHtree.h", 11,
                  typeid(::RHtree), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RHtree::Dictionary, isa_proxy, 4,
                  sizeof(::RHtree) );
      instance.SetNew(&new_RHtree);
      instance.SetNewArray(&newArray_RHtree);
      instance.SetDelete(&delete_RHtree);
      instance.SetDeleteArray(&deleteArray_RHtree);
      instance.SetDestructor(&destruct_RHtree);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RHtree*)
   {
      return GenerateInitInstanceLocal(static_cast<::RHtree*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::RHtree*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *RHfit_Dictionary();
   static void RHfit_TClassManip(TClass*);
   static void delete_RHfit(void *p);
   static void deleteArray_RHfit(void *p);
   static void destruct_RHfit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RHfit*)
   {
      ::RHfit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::RHfit));
      static ::ROOT::TGenericClassInfo 
         instance("RHfit", "include/RHfit.h", 11,
                  typeid(::RHfit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &RHfit_Dictionary, isa_proxy, 4,
                  sizeof(::RHfit) );
      instance.SetDelete(&delete_RHfit);
      instance.SetDeleteArray(&deleteArray_RHfit);
      instance.SetDestructor(&destruct_RHfit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RHfit*)
   {
      return GenerateInitInstanceLocal(static_cast<::RHfit*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::RHfit*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *RHfit_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::RHfit*>(nullptr))->GetClass();
      RHfit_TClassManip(theClass);
   return theClass;
   }

   static void RHfit_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_HistogramHelper(void *p = nullptr);
   static void *newArray_HistogramHelper(Long_t size, void *p);
   static void delete_HistogramHelper(void *p);
   static void deleteArray_HistogramHelper(void *p);
   static void destruct_HistogramHelper(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HistogramHelper*)
   {
      ::HistogramHelper *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HistogramHelper >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("HistogramHelper", ::HistogramHelper::Class_Version(), "include/HistogramHelper.h", 17,
                  typeid(::HistogramHelper), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HistogramHelper::Dictionary, isa_proxy, 4,
                  sizeof(::HistogramHelper) );
      instance.SetNew(&new_HistogramHelper);
      instance.SetNewArray(&newArray_HistogramHelper);
      instance.SetDelete(&delete_HistogramHelper);
      instance.SetDeleteArray(&deleteArray_HistogramHelper);
      instance.SetDestructor(&destruct_HistogramHelper);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HistogramHelper*)
   {
      return GenerateInitInstanceLocal(static_cast<::HistogramHelper*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::HistogramHelper*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RHtree::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RHtree::Class_Name()
{
   return "RHtree";
}

//______________________________________________________________________________
const char *RHtree::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RHtree*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RHtree::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RHtree*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RHtree::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RHtree*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RHtree::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RHtree*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HistogramHelper::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HistogramHelper::Class_Name()
{
   return "HistogramHelper";
}

//______________________________________________________________________________
const char *HistogramHelper::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HistogramHelper*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HistogramHelper::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HistogramHelper*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HistogramHelper::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HistogramHelper*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HistogramHelper::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HistogramHelper*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RHtree::Streamer(TBuffer &R__b)
{
   // Stream an object of class RHtree.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RHtree::Class(),this);
   } else {
      R__b.WriteClassBuffer(RHtree::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RHtree(void *p) {
      return  p ? new(p) ::RHtree : new ::RHtree;
   }
   static void *newArray_RHtree(Long_t nElements, void *p) {
      return p ? new(p) ::RHtree[nElements] : new ::RHtree[nElements];
   }
   // Wrapper around operator delete
   static void delete_RHtree(void *p) {
      delete (static_cast<::RHtree*>(p));
   }
   static void deleteArray_RHtree(void *p) {
      delete [] (static_cast<::RHtree*>(p));
   }
   static void destruct_RHtree(void *p) {
      typedef ::RHtree current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::RHtree

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RHfit(void *p) {
      delete (static_cast<::RHfit*>(p));
   }
   static void deleteArray_RHfit(void *p) {
      delete [] (static_cast<::RHfit*>(p));
   }
   static void destruct_RHfit(void *p) {
      typedef ::RHfit current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::RHfit

//______________________________________________________________________________
void HistogramHelper::Streamer(TBuffer &R__b)
{
   // Stream an object of class HistogramHelper.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(HistogramHelper::Class(),this);
   } else {
      R__b.WriteClassBuffer(HistogramHelper::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HistogramHelper(void *p) {
      return  p ? new(p) ::HistogramHelper : new ::HistogramHelper;
   }
   static void *newArray_HistogramHelper(Long_t nElements, void *p) {
      return p ? new(p) ::HistogramHelper[nElements] : new ::HistogramHelper[nElements];
   }
   // Wrapper around operator delete
   static void delete_HistogramHelper(void *p) {
      delete (static_cast<::HistogramHelper*>(p));
   }
   static void deleteArray_HistogramHelper(void *p) {
      delete [] (static_cast<::HistogramHelper*>(p));
   }
   static void destruct_HistogramHelper(void *p) {
      typedef ::HistogramHelper current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::HistogramHelper

namespace ROOT {
   static TClass *vectorlEvectorlEfloatgRsPgR_Dictionary();
   static void vectorlEvectorlEfloatgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEfloatgRsPgR(void *p = nullptr);
   static void *newArray_vectorlEvectorlEfloatgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEfloatgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEfloatgRsPgR(void *p);
   static void destruct_vectorlEvectorlEfloatgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<float> >*)
   {
      vector<vector<float> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<float> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<float> >", -2, "vector", 389,
                  typeid(vector<vector<float> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEfloatgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<float> >) );
      instance.SetNew(&new_vectorlEvectorlEfloatgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEfloatgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEfloatgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEfloatgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEfloatgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<float> > >()));

      ::ROOT::AddClassAlternate("vector<vector<float> >","std::vector<std::vector<float, std::allocator<float> >, std::allocator<std::vector<float, std::allocator<float> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<vector<float> >*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEfloatgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<vector<float> >*>(nullptr))->GetClass();
      vectorlEvectorlEfloatgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEfloatgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEfloatgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<float> > : new vector<vector<float> >;
   }
   static void *newArray_vectorlEvectorlEfloatgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<float> >[nElements] : new vector<vector<float> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEfloatgRsPgR(void *p) {
      delete (static_cast<vector<vector<float> >*>(p));
   }
   static void deleteArray_vectorlEvectorlEfloatgRsPgR(void *p) {
      delete [] (static_cast<vector<vector<float> >*>(p));
   }
   static void destruct_vectorlEvectorlEfloatgRsPgR(void *p) {
      typedef vector<vector<float> > current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<vector<float> >

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 389,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<float>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<float>*>(nullptr))->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete (static_cast<vector<float>*>(p));
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] (static_cast<vector<float>*>(p));
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace {
  void TriggerDictionaryInitialization_libRHelperDict_Impl() {
    static const char* headers[] = {
"/home/malin/hades/software/src/RHelper/include/HistogramHelper.h",
"/home/malin/hades/software/src/RHelper/include/RHtree.h",
"/home/malin/hades/software/src/RHelper/include/RHfit.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/malin/hades/software/install/root/include",
"/home/malin/hades/software/src/RHelper",
"/home/malin/hades/software/install/root/include/",
"/home/malin/hades/software/src/RHelper/build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libRHelperDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$/home/malin/hades/software/src/RHelper/include/HistogramHelper.h")))  RHtree;
class __attribute__((annotate("$clingAutoload$/home/malin/hades/software/src/RHelper/include/HistogramHelper.h")))  RHfit;
class __attribute__((annotate("$clingAutoload$/home/malin/hades/software/src/RHelper/include/HistogramHelper.h")))  HistogramHelper;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRHelperDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/malin/hades/software/src/RHelper/include/HistogramHelper.h"
#include "/home/malin/hades/software/src/RHelper/include/RHtree.h"
#include "/home/malin/hades/software/src/RHelper/include/RHfit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"HistogramHelper", payloadCode, "@",
"RHfit", payloadCode, "@",
"RHtree", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRHelperDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRHelperDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRHelperDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRHelperDict() {
  TriggerDictionaryInitialization_libRHelperDict_Impl();
}
