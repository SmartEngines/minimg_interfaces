/*

Copyright 2021 Smart Engines Service LLC

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#pragma once
#ifndef MINUTILS_MINHELPERS_H_INCLUDED
#define MINUTILS_MINHELPERS_H_INCLUDED

/// Macro for disallowing copy constructor and assignment operator for
/// @ClassName.
#define DISALLOW_COPY_AND_ASSIGN(ClassName)     \
  private:\
  ClassName(const ClassName &); \
  void operator=(const ClassName &);

/// Macro for easy defining a member with its getter and setter methods.o
#define DEFINE_GET_SET_MEMBER(Type,name)                   \
  public:                                                  \
  const Type & name() const { return name##_; }            \
   void set_##name(const Type &value) { name##_ = value; } \
 private:                                                  \
   Type name##_;

/// Some times it is necessary to have a variable, which will not be used in
/// some the context. For example if the code which uses the variable is under
/// conditional compilation. For such cases it is convenient to use this macro
/// to suppress corresponding warnings.
///
/// Remark: use in case of strong need only. Remove unused variables if they
/// are not needed at all.
#define SUPPRESS_UNUSED_VARIABLE(x) (void)(x);


/// Macro to mark a variable as unused right inside the function signature.
#define UNUSED_VAR(x)

#endif // #ifndef MINUTILS_MINHELPERS_H_INCLUDED
