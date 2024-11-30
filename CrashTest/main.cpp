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

#include <LadyBug.hpp>
#include <thread>

void foo();
void bar();
void loop();

int main()
{
    LadyBug::initialize();
    
    for( int i = 0; i < 10; i++ )
    {
        std::thread
        (
            []
            {
                loop();
            }
        )
        .detach();
    }
    
    std::thread
    (
        []
        {
            foo();
        }
    )
    .join();
    
    return 0;
}

void foo()
{
    bar();
}

void bar()
{
    volatile int x = 0;
    
    if( x == 0 )
    {
        throw std::runtime_error( "hello, world" );
    }
}

void loop()
{
    volatile int x = 0;
    
    if( x == 0 )
    {
        while( 1 );
    }
}
