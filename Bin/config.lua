entity={
["DriverName"]="Micro Unidrv",
["PrinterName"]="Micro Unidrv",
["Manufacturer"]="achellies",
["DriverPath"]="UNIDRV.DLL",
["ConfigFile"]="UNIDRVUI.DLL",
["HelpFile"]="UNIDRV.HLP",
["DataFile"]="Microdrv.GPD",
["DataFileDLL"]="Microdrv.Dll",
["OEMConfigFile"]="MicrodrvUI.dll",
["OEMDriverFile"]="MicrodrvUNI.dll",
["OEMPlugFile"] = "UnidrvPlug.ini",
["OEMHelpFile"] = "Microdrv.HLP"
}

function getDriverConfig(name)
	return entity[name]
end