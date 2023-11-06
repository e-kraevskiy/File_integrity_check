
#include <unordered_map>

#include "utils.hpp"


std::string CompareStatusToString( CompareStatus status )
{
   static std::unordered_map< CompareStatus, std::string > status_to_str = { { CompareStatus::OK, "OK" },
                                                                             { CompareStatus::FAIL, "FAIL" },
                                                                             { CompareStatus::NEW, "NEW" },
                                                                             { CompareStatus::ABSENT, "ABSENT" } };
   return status_to_str[ status ];
}
