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

#ifndef LADYBUG_REGISTER_HPP
#define LADYBUG_REGISTER_HPP

#include <memory>
#include <algorithm>
#include <vector>
#include <cstdint>

namespace LadyBug
{
    class Register
    {
        public:
            
            enum class Size
            {
                R8,
                R16,
                R32,
                R64
            };
            
            Register( const std::string & name, uint64_t value, Size size );
            Register( const Register & o );
            ~Register();
            
            Register & operator =( Register o );
            
            friend void swap( Register & o1, Register & o2 );
            
            std::string name()     const;
            std::string hexValue() const;
            uint64_t    value()    const;
            Size        size()     const;
            
        private:
        
            class IMPL;
            
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* LADYBUG_REGISTER_HPP */
