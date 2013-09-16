//------------------------------------------------------------------------------
// Author: Dzianis Huznou
// Description: Plugin which encapsulate all requests to shared object library.
// Copyright (c) 2013 EPAM Systems. All Rights Reserved.
//------------------------------------------------------------------------------
#include "plugin.h"
#include "../auxiliary/exception.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace NST
{
namespace analyzer
{

Plugin::Plugin(const std::string& path)
    : DynamicLoad(path.c_str())
    , usage(NULL)
    , create(NULL)
    , destroy(NULL)
{
    load_address_of("usage" ,  usage  );
    load_address_of("create" , create );
    load_address_of("destroy", destroy);
}

Plugin::~Plugin()
{
}


PluginInstance::PluginInstance(const std::string& path, const std::string& args) : Plugin(path)
{
    analyzer = (BaseAnalyzer*)(*create)(args.c_str());
    if(!analyzer)
    {
        throw NST::auxiliary::Exception(path + ": create call returns NULL-pointer");
    }
}

PluginInstance::~PluginInstance()
{
    (*destroy)(analyzer);
}

} // namespace analyzer
} // namespace NST
//------------------------------------------------------------------------------