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

#include <LadyBug/Formatter.hpp>
#include <LadyBug/Thread.hpp>
#include <LadyBug/Register.hpp>
#include <LadyBug/Vector.hpp>
#include <LadyBug/String.hpp>
#include <string>
#include <sstream>

namespace LadyBug
{
    namespace Formatter
    {
        std::string thread( const Thread & thread )
        {
            std::stringstream ss;
            
            if( thread.name().length() > 0 )
            {
                ss << "Thread " << thread.number() << " (" << thread.name() << "):" << std::endl;
            }
            else
            {
                ss << "Thread " << thread.number() << ":" << std::endl;
            }
            
            ss << registers( thread.registers() );
            
            return ss.str();
        }
        
        std::string registers( const std::vector< Register > & registers )
        {
            std::stringstream ss;
            
            size_t longest = 0;
            
            for( const auto & reg: registers )
            {
                longest = std::max( longest, reg.name().length() );
            }
            
            for( const auto & p: Vector::enumerated( registers ) )
            {
                Register    reg   = p.second;
                std::string pad   = std::string( longest - reg.name().length(), ' ' );
                int         width = 0;
                
                switch( reg.size() )
                {
                    case Register::Size::R8:  width = 2;  break;
                    case Register::Size::R16: width = 4;  break;
                    case Register::Size::R32: width = 8;  break;
                    case Register::Size::R64: width = 16; break;
                }
                
                ss << reg.name() << ": " << pad << String::toHex( reg.value(), width );
                
                if( ( p.first + 1 ) % 4 == 0 )
                {
                    ss << std::endl;
                }
                else
                {
                    ss << "  ";
                }
            }
            
            return ss.str();
        }
    }
}
