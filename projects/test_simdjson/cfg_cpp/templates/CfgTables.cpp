// author: uniqs
// uniqs@163.com
// ====================== This file is generated by uniqs, don't change it!!!!!! ====================== 



#include "CfgTables.h"

{{%Loop.sheet%}}
#include "DT{{%=SheetName%}}.h"
{{%LoopEnd.sheet%}}

{{%Loop.sheet%}}
std::unique_ptr<DTCfgTable<{{%=row:col.2:1}}, DT{{%=SheetName%}}>> g_pCfg_{{%=SheetName%}} = nullptr;
{{%LoopEnd.sheet%}}

{{%Loop.sheet%}}
bool LoadDTTables_{{%=SheetName%}}(const char* szFile, std::string& err)
{
	if (!g_pCfg_{{%=SheetName%}})
	{
		g_pCfg_{{%=SheetName%}} = std::make_unique<DTCfgTable<{{%=row:col.2:1}}, DT{{%=SheetName%}}>>();
	}

	if (!g_pCfg_{{%=SheetName%}}->Load(szFile, err)) return false;

	return true;
}
{{%LoopEnd.sheet%}}

bool LoadDTTables(const std::string& prefix, std::string& err)
{
	std::string file;

{{%Loop.sheet%}}
	file = prefix + "DT{{%=SheetName%}}.json";
	if (!LoadDTTables_{{%=SheetName%}}(file.c_str(), err)) return false;

{{%LoopEnd.sheet%}}

	return true;
}