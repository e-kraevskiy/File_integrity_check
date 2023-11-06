#pragma once

#include "../file_check_sum_calculator.hpp"

class SysLogger
{
public:
   SysLogger();

   void Log( const FileCompareResult& result ) const;
   void LogError( const std::string& error_msg ) const;

   ~SysLogger();

private:
   std::string GetErrorMessage( const FileCompareResult& result ) const;

};


