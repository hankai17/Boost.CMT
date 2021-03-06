#include <boost/cmt/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/cmt/log/log.hpp>
#include <boost/context/all.hpp>
#include <boost/cmt/signals.hpp>

using namespace boost::cmt;



boost::signal<void(std::string)> test_signal;

int goodbye(const std::string& world ) {
//    slog( "goodbye %1%", world );
    return 5;//world.size();
}
void bench();

void_t delay()
{
    slog( "delay 3 sec, %1%", 3.13);
    boost::cmt::usleep(3000000);
    slog( "test_signal");
    test_signal("hello world!");
	return void_t();
}
int hello2(int cnt) {
    slog( "%1%", cnt );    
    return cnt;
}

int hello(int cnt) {
    return cnt;
    slog( "%1%", cnt );    
    boost::cmt::async<int>( boost::bind(hello2,cnt+10) );
    boost::cmt::async<int>( boost::bind(hello2,cnt+1000) );
    boost::cmt::async<int>( boost::bind(hello2,cnt+10000) ).wait();
    return boost::cmt::sync<int>( boost::bind(hello2,cnt+100) );
}

void main2() {
 //   slog( "%1%", *tmp );
    /*
    future<int> rtn  = async<int>( boost::bind(hello, "world"), "hello_func" );
    future<int> rtn2 = async<int>( boost::bind(hello, "world2"), "hello_func2" );
    std::cerr<<"length: "<<rtn.wait()<<std::endl;
    future<int> rtn3 = async<int>( boost::bind(hello, "world3"), "hello_func3" );
    future<int> rtn4 = async<int>( boost::bind(hello, "world4____"), "hello_func4" );
    std::cerr<<"length: "<<rtn4.wait()<<std::endl;
    future<int> rtn5 = async<int>( boost::bind(hello, "world5"), "hello_func5" );
    wlog( "main2 done" );
    */
}

void bench() {
  wlog( "waiting async delay" );
    async<void_t>(delay).wait();
  wlog( "delay returned!" );
    uint64_t cnt = 1000000;
    //uint64_t cnt = 10;
    wlog( "done waiting on delay" );
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
    std::string hellostr("hello");
    for( uint32_t i = 0; i < cnt; ++i ) {
        try {
            async<int>( boost::bind(hello, i), "hello_func" ).wait(boost::chrono::seconds(1));
           // async<int>( boost::bind(hello, "world"), "hello_func" ).wait();//2000000);
        } catch( const boost::exception& e ) {
                elog( "%1%", boost::diagnostic_information(e) );
        }
    }
    boost::posix_time::ptime stop = boost::posix_time::microsec_clock::universal_time();
    slog( "%1% calls/sec", ((cnt*1.0)/((stop-start).total_microseconds()/1000000.0)) );


    start = boost::posix_time::microsec_clock::universal_time();
    for( uint32_t i = 0; i < cnt; ++i ) {
        try {
           // async<int>( boost::bind(hello, "world"), "hello_func" ).wait(1000000);
            async<int>( boost::bind(hello, i), "hello_func" ).wait();//2000000);
        } catch( const boost::exception& e ) {
                elog( "%1%", boost::diagnostic_information(e) );
        }
    }
    stop = boost::posix_time::microsec_clock::universal_time();
    slog( "%1% calls/sec", ((cnt*1.0)/((stop-start).total_microseconds()/1000000.0)) );


    start = boost::posix_time::microsec_clock::universal_time();
    for( uint32_t i = 0; i < cnt; ++i ) {
        try {
           // async<int>( boost::bind(hello, "world"), "hello_func" ).wait(1000000);
            sync<int>( boost::bind(hello, i), "hello_func" );//2000000);
        } catch( const boost::exception& e ) {
                elog( "%1%", boost::diagnostic_information(e) );
        }
    }
    stop = boost::posix_time::microsec_clock::universal_time();
    slog( "%1% calls/sec", ((cnt*1.0)/((stop-start).total_microseconds()/1000000.0)) );


    int group_size = 10;
    cnt/=group_size;
    std::vector< boost::cmt::future<int> > futures(10);
    start = boost::posix_time::microsec_clock::universal_time();
    for( uint32_t i = 0; i < cnt; ++i ) {
        try {
           // async<int>( boost::bind(hello, "world"), "hello_func" ).wait(1000000);
           for( uint32_t f = 0; f < group_size; ++f ) {
            futures[f] = async<int>( boost::bind(hello, i*group_size+f), "hello_func", boost::cmt::priority(f) );//2000000);
           }
           for( uint32_t f = 0; f < group_size; ++f ) {
            futures[f].wait();
           }
        } catch( const boost::exception& e ) {
                elog( "%1%", boost::diagnostic_information(e) );
        }
    }
    stop = boost::posix_time::microsec_clock::universal_time();
    slog( "%1% calls/sec", ((group_size*cnt*1.0)/((stop-start).total_microseconds()/1000000.0)) );


    start = boost::posix_time::microsec_clock::universal_time();
    for( uint32_t i = 0; i < cnt; ++i ) {
        try {
           // async<int>( boost::bind(hello, "world"), "hello_func" ).wait(1000000);
           for( uint32_t f = 0; f < group_size; ++f ) {
            futures[f] = async<int>( boost::bind(hello, i*group_size+f), "hello_func" );//2000000);
           }
           for( uint32_t f = 0; f < group_size; ++f ) {
            futures[f].wait();
           }
        } catch( const boost::exception& e ) {
                elog( "%1%", boost::diagnostic_information(e) );
        }
    }
    stop = boost::posix_time::microsec_clock::universal_time();
    slog( "%1% calls/sec", ((group_size*cnt*1.0)/((stop-start).total_microseconds()/1000000.0)) );

    slog( "done" );
}

int main( int argc, char** argv )
{
  bench();
}
#if 0
    slog( "%1%", 1.234 );
    try {
    /*
    a = new context_t( boost::bind(&hello,"A",(context_t*)NULL), 
                   bc::protected_stack( bc::stack_helper::default_stacksize() ), true, true );
    b = new context_t( boost::bind(&hello,"B",a), 
                   bc::protected_stack( bc::stack_helper::default_stacksize() ), true, true );
    c = new context_t( boost::bind(&hello,"C",b), 
                   bc::protected_stack( bc::stack_helper::default_stacksize() ), true, true );

    c->resume();
    slog( "done" );
    */
//    slog( "test usleep 1s" );
//    boost::cmt::thread::current().usleep(1000000);
//    slog( "done usleep 1s" );
//    bench();
    async(bench);
   // async(main2);
    boost::cmt::exec();
    } catch( const boost::exception& e ) {
        elog( "%1%", boost::diagnostic_information(e) );
    }
}

#endif
