#include "sys_logger.hpp"

#include "syslog.h"
#include <string>
#include <iostream>

const static std::string LOG_PREFIX = "Integrity check: ";
const std::string SYSLOG_PROGRAM_NAME = "File_integrity_check";

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
   res.append( "FAIL (" );
   res.append( result.mAbsolutePath );
   res.append( " - " );
   switch ( result.mStatus )
   {
      case CompareStatus::FAIL:
      {
         res.append( std::to_string( result.mOldCrc ) );
         res.push_back( ',' );
         res.append( std::to_string( result.mNewCrc ) );
         break;
      }
      case CompareStatus::NEW:
         res.append( "new file" );
         break;
      case CompareStatus::ABSENT:
         res.append( "not found" );
         break;
      case CompareStatus::OK:
         std::cout << "SysLogger::GetErrorMessage. Error: CompareStatus::OK shouldn't be there." << std::endl;
         break;
   }
   res += ")";

   return res;
}

void SysLogger::LogError( const std::string& error_msg ) const
{
   syslog( LOG_ERR, "%s", error_msg.c_str() );
}
