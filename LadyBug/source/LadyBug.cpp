/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2024 Jean-David Gadina - www.xs-labs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include <LadyBug/LadyBug.hpp>
#include <LadyBug/Thread.hpp>
#include <LadyBug/Register.hpp>
#include <LadyBug/Vector.hpp>
#include <LadyBug/String.hpp>
#include <LadyBug/Formatter.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

namespace LadyBug
{
    void handleCPPException() __attribute__( ( noreturn ) );
    
    void initialize()
    {
        std::set_terminate
        (
            []
            {
                handleCPPException();
            }
        );
    }
    
    void handleCPPException()
    {
        std::vector< Thread > threads = Thread::all();
        
        for( auto & thread: threads )
        {
            if( thread.isCurrent() == false )
            {
                thread.suspend();
            }
        }
        
        std::exception_ptr exception{ std::current_exception() };
        
        if( exception == nullptr )
        {
            std::cerr << "Unknown uncaught exception" << std::endl;
        }
        else
        {
            try
            {
                std::rethrow_exception( exception );
            }
            catch( const std::exception & e )
            {
                std::cerr << "Uncaught exception: " << e.what() << std::endl;
            }
            catch( ... )
            {
                std::cerr << "Unknown uncaught exception" << std::endl;
            }
        }
        
        for( const auto & thread: threads )
        {
            if( thread.isCurrent() == false )
            {
                std::cerr << Formatter::thread( thread ) << std::endl;
            }
        }
        
        std::abort();
    }
}
