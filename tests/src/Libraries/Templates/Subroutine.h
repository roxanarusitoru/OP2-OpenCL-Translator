


/*  Open source copyright declaration based on BSD open source template:
 *  http://www.opensource.org/licenses/bsd-license.php
 * 
 * Copyright (c) 2011-2012, Adam Betts, Carlo Bertolli
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Written by Adam Betts and Carlo Bertolli
 *
 * This class models subroutines to be generated by our
 * compiler
 */

#pragma once
#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include <boost/lexical_cast.hpp>
#include <boost/crc.hpp>
#include <ScopedVariableDeclarations.h>
#include <Debug.h>
#include <rose.h>

template <typename TSubroutineHeader>
  class Subroutine
  {
    protected:

      /*
       * ======================================================
       * Access to this statement allows ROSE to build function
       * call expressions to the generated subroutine. This
       * is needed, for example, when patching the user-supplied
       * code
       * ======================================================
       */
      TSubroutineHeader * subroutineHeaderStatement;

      /*
       * ======================================================
       * The name of the generated subroutine
       * ======================================================
       */
      std::string subroutineName;

      /*
       * ======================================================
       * The parameters of the newly created subroutine
       * ======================================================
       */
      SgFunctionParameterList * formalParameters;

      /*
       * ======================================================
       * The scope of the newly created subroutine
       * ======================================================
       */
      SgScopeStatement * subroutineScope;

      /*
       * ======================================================
       * Formal parameter and local variable declarations in the
       * newly created subroutine
       * ======================================================
       */
      ScopedVariableDeclarations * variableDeclarations;

    protected:

      /*
       * ======================================================
       * Every created subroutine has a number of statements
       * ======================================================
       */

      virtual void
      createStatements () = 0;

      /*
       * ======================================================
       * Every created subroutine has local variable declarations
       * ======================================================
       */

      virtual void
      createLocalVariableDeclarations () = 0;

      /*
       * ======================================================
       * Every created subroutine has formal parameters
       * ======================================================
       */

      virtual void
      createFormalParameterDeclarations () = 0;

      /*
       * ======================================================
       * Subroutine constructor with name of the subroutine to
       * create
       * ======================================================
       */

      Subroutine (std::string const & subroutineName)
      {
        using boost::lexical_cast;
        using SageBuilder::buildFunctionParameterList;
        using std::string;

        if (subroutineName.length () > 20)
        {
          boost::crc_32_type result;

          result.process_bytes (subroutineName.c_str (),
              subroutineName.length ());

          this->subroutineName = "s" + lexical_cast <string> (
              result.checksum ());
        }
        else
        {
          this->subroutineName = subroutineName;
        }

        Debug::getInstance ()->debugMessage ("Subroutine name = "
            + this->subroutineName, Debug::CONSTRUCTOR_LEVEL, __FILE__,
            __LINE__);

        formalParameters = buildFunctionParameterList ();

        variableDeclarations = new ScopedVariableDeclarations ();
      }

    public:

      /*
       * ======================================================
       * Returns the header statement of the subroutine used
       * internally by ROSE in its abstract syntax tree
       * ======================================================
       */

      TSubroutineHeader *
      getSubroutineHeaderStatement ()
      {
        return subroutineHeaderStatement;
      }

      /*
       * ======================================================
       * What is the name of the subroutine?
       * ======================================================
       */

      std::string const &
      getSubroutineName () const
      {
        return subroutineName;
      }

      /*
       * ======================================================
       * Returns the variable declarations in this subroutine
       * ======================================================
       */

      ScopedVariableDeclarations *
      getVariableDeclarations ()
      {
        return variableDeclarations;
      }
  };

#endif
