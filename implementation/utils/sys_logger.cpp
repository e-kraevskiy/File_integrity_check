#include "sys_logger.hpp"

#include "syslog.h"
#include <string>
#include <iostream>

#include "config.hpp"

const static std::string LOG_PREFIX = "Integrity check: ";

SysLogger::SysLogger()
{
   openlog( SYSLOG_PROGRAM_NAME.c_str(), LOG_PID, LOG_USER );
}

void SysLogger::Log( const FileCompareResult& result ) const
{
   if ( result.mStatus == CompareStatus::OK )
   {
      std::string log_str = LOG_PREFIX + "OK";
      syslog( LOG_INFO, "%s", log_str.c_str() );
      return;
   }
   syslog( LOG_INFO, "%s", GetErrorMessage( result ).c_str() );
}

SysLogger::~SysLogger()
{
   closelog();
}

std::string SysLogger::GetErrorMessage( const FileCompareResult& result ) const
{
   std::string res = LOG_PREFIX;
   res += "FAIL (";
   res += result.mAbsolutePath;
   res += " - ";
   switch ( result.mStatus )
   {
      case CompareStatus::FAIL:
      {
         res += std::to_string( result.mOldCrc );
         res.push_back( ',' );
         res += std::to_string( result.mNewCrc );
      }
      case CompareStatus::NEW:
         res += "new file";
      case CompareStatus::ABSENT:
         res += "not found";
      case CompareStatus::OK:
         std::cout << "SysLogger::GetErrorMessage. Error: CompareStatus::OK shouldn't be there.";
   }
   res += ")";

   return res;
}
