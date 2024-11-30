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

#include <LadyBug/Register.hpp>
#include <LadyBug/String.hpp>

namespace LadyBug
{
    class Register::IMPL
    {
        public:
            
            IMPL( const std::string & name, uint64_t value, Size size );
            IMPL( const IMPL & o );
            
            std::string _name;
            uint64_t    _value;
            Size        _size;
    };
    
    Register::Register( const std::string & name, uint64_t value, Size size ):
        impl( std::make_unique< IMPL >( name, value, size ) )
    {}
    
    Register::Register( const Register & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    Register::~Register()
    {}
    
    Register & Register::operator =( Register o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    void swap( Register & o1, Register & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    std::string Register::name() const
    {
        return this->impl->_name;
    }
    
    std::string Register::hexValue() const
    {
        switch( this->impl->_size )
        {
            case Size::R8:  return String::toHex( this->impl->_value, 2 );
            case Size::R16: return String::toHex( this->impl->_value, 4 );
            case Size::R32: return String::toHex( this->impl->_value, 8 );
            case Size::R64: return String::toHex( this->impl->_value, 16 );
        }
    }
    
    uint64_t Register::value() const
    {
        return this->impl->_value;
    }
    
    Register::Size Register::size() const
    {
        return this->impl->_size;
    }
    
    Register::IMPL::IMPL( const std::string & name, uint64_t value, Size size ):
        _name(  name ),
        _value( value ),
        _size(  size )
    {}
    
    Register::IMPL::IMPL( const IMPL & o ):
        _name(  o._name ),
        _value( o._value ),
        _size(  o._size )
    {}
}
