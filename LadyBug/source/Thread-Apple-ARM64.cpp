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

#include <LadyBug/Thread.hpp>
#include <LadyBug/Register.hpp>
#include <mach/mach.h>

namespace LadyBug
{
    class Thread::IMPL
    {
        public:
            
            IMPL( thread_t thread, size_t number );
            IMPL( const IMPL & o );
            
            thread_t    _thread;
            size_t      _number;
            std::string _name;
    };
    
    std::vector< Thread > Thread::all()
    {
        task_t   currentTask   = mach_task_self();
        thread_t currentThread = mach_thread_self();
        
        mach_port_deallocate( currentTask, currentThread );
        
        thread_act_array_t     threads     = nullptr;
        mach_msg_type_number_t threadCount = 0;
        kern_return_t          result      = task_threads( currentTask, &threads, &threadCount );
        
        if( result != KERN_SUCCESS || threads == nullptr || threadCount == 0 )
        {
            return {};
        }
        
        std::vector< Thread > all;

        for( mach_msg_type_number_t i = 0; i < threadCount; i++ )
        {
            all.push_back( std::make_unique< IMPL >( threads[ i ], i ) );
            mach_port_deallocate( currentTask, threads[ i ] );
        }
        
        vm_deallocate( currentTask, reinterpret_cast< vm_address_t >( threads ), sizeof( thread_t ) * threadCount );
        
        return all;
    }
    
    Thread::Thread( const std::unique_ptr< IMPL > & i ):
        impl( std::make_unique< IMPL >( *( i ) ) )
    {}
    
    Thread::Thread( const Thread & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    Thread::~Thread()
    {}
    
    Thread & Thread::operator =( Thread o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    void swap( Thread & o1, Thread & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    bool Thread::suspend()
    {
        return thread_suspend( this->impl->_thread ) == KERN_SUCCESS;
    }
    
    bool Thread::resume()
    {
        return thread_resume( this->impl->_thread ) == KERN_SUCCESS;
    }
    
    bool Thread::isCurrent() const
    {
        thread_t current = mach_thread_self();
        
        mach_port_deallocate( mach_task_self(), current );
        
        return this->impl->_thread == current;
    }
    
    size_t Thread::number() const
    {
        return this->impl->_number;
    }
    
    std::string Thread::name() const
    {
        return this->impl->_name;
    }
    
    std::vector< Register > Thread::registers() const
    {
        arm_thread_state64_t   state;
        mach_msg_type_number_t count = ARM_THREAD_STATE64_COUNT;
        
        memset( &state, 0, sizeof( state ) );
        
        if( thread_get_state( this->impl->_thread, ARM_THREAD_STATE64, reinterpret_cast< thread_state_t >( &state ), &count ) != KERN_SUCCESS )
        {
            return {};
        }
        
        std::vector< Register > registers;
        
        for( size_t i = 0; i < sizeof( state.__x ) / sizeof( uint64_t ); i++ )
        {
            registers.push_back( { "x" + std::to_string( i ), state.__x[ i ], Register::Size::R64 } );
        }
        
        registers.push_back( { "fp",   state.__fp,   Register::Size::R64 } );
        registers.push_back( { "lr",   state.__lr,   Register::Size::R64 } );
        registers.push_back( { "sp",   state.__sp,   Register::Size::R64 } );
        registers.push_back( { "pc",   state.__pc,   Register::Size::R64 } );
        registers.push_back( { "cpsr", state.__cpsr, Register::Size::R32 } );
        
        return registers;
    }
    
    Thread::IMPL::IMPL( thread_t thread, size_t number ):
        _thread( thread ),
        _number( number )
    {
        thread_extended_info_data_t info;
        mach_msg_type_number_t      count = THREAD_EXTENDED_INFO_COUNT;
        
        memset( &info, 0, sizeof( info ) );
        
        if( thread_info( thread, THREAD_EXTENDED_INFO, reinterpret_cast< thread_info_t >( &info ), &count ) == KERN_SUCCESS )
        {
            this->_name = info.pth_name;
        }
    }
    
    Thread::IMPL::IMPL( const IMPL & o ):
        _thread( o._thread ),
        _number( o._number ),
        _name(   o._name )
    {}
}
