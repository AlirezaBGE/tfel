/*!
 * \file   basic_rstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 nov 2007
 */

#include<cerrno>
#include<cstring>
#include<limits>

#include<unistd.h>

#include"TFEL/System/basic_rstream.hxx"
#include"TFEL/System/System.hxx"

namespace tfel
{

  namespace system
  {

    void
    BlockingStreamReader::read(int fd,void *const buf,const size_t count)
    {
      using namespace std;
      ssize_t rread;
      if(count>static_cast<size_t>(numeric_limits<ssize_t>::max())){
	throw(SystemError("BlockingStreamReader::read : number of bytes to be read too high"));
      }
      rread=::read(fd,buf,count);
      while(rread==-1){
	if(errno!=EINTR){
	  systemCall::throwSystemError("BlockingStreamReader::read : read failed",errno);
	}
	rread=::read(fd,buf,count);
      }
    } // end of BlockingStreamReader::read

    void
    NonBlockingStreamReader::read(int fd,void *const buf,const size_t count)
    {
      using namespace std;
      ssize_t rread;
      if(count>static_cast<size_t>(numeric_limits<ssize_t>::max())){
	throw(SystemError("NonBlockingStreamReader::read : number of bytes to be read too high"));
      }
      rread=::read(fd,buf,count);
      while(rread==-1){
	if(errno!=EINTR){
	  systemCall::throwSystemError("BlockingStreamReader::read : read failed",errno);
	}
	rread=::read(fd,buf,count);
      }
    } // end of NonBlockingStreamReader::read

  } // end of namespace System  
  
} // end of namespace tfel  

