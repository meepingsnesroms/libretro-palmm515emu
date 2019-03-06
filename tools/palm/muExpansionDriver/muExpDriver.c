#include "sdkPatch/PalmOSPatched.h"
#include <stdint.h>

#include "debug.h"
#include "config.h"
#include "gui.h"
#include "patcher.h"


static void setConfigDefaults(uint32_t* configFile){
   debugLog("First load, setting default config.\n");
   
   configFile[USER_WARNING_GIVEN] = false;
   configFile[ARM_STACK_SIZE] = 0x4000;
   configFile[LCD_WIDTH] = 160;
   configFile[LCD_HEIGHT] = 220;
   configFile[BOOT_CPU_SPEED] = 100;
   configFile[EXTRA_RAM_MB_DYNAMIC_HEAP] = 10;
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdBPB, UInt16 launchFlags){
   DmOpenRef configDb;
   MemHandle configHandle;
   uint32_t* dmConfigFile;
   uint32_t configFile[CONFIG_FILE_ENTRIES];
   Err error;
   
   configDb = DmOpenDatabaseByTypeCreator('EMUC', APP_CREATOR, dmModeReadWrite);
   
   /*create db and set defaults if config doesnt exist*/
   if(!configDb){
      error = DmCreateDatabase(0/*cardNo*/, "Emu Config", APP_CREATOR, 'EMUC', true);
      if(error != errNone)
         debugLog("Tried to create db, err:%d\n", error);
      
      configDb = DmOpenDatabaseByTypeCreator('EMUC', APP_CREATOR, dmModeReadWrite);
      if(!configDb)
         debugLog("Cant find created db!\n");
      
      configHandle = DmNewResource(configDb, 'CONF', 0, CONFIG_FILE_ENTRIES * sizeof(uint32_t));
      if(!configHandle)
         debugLog("Cant open db resource!\n");
      
      dmConfigFile = MemHandleLock(configHandle);
      setConfigDefaults(configFile);
   }
   else{
      configHandle = DmGetResource('CONF', 0);
      if(!configHandle)
         debugLog("Cant open db resource!\n");
      
      dmConfigFile = MemHandleLock(configHandle);
      MemMove(configFile, dmConfigFile, CONFIG_FILE_ENTRIES * sizeof(uint32_t));
   }
   
   if(cmd == sysAppLaunchCmdNormalLaunch)
      showGui(configFile);
   else if(cmd == sysAppLaunchCmdSystemReset)
      initBoot(configFile);
   
   /*must use DmWrite to write to databases or a write protect violation may trigger*/
   error = DmWrite(dmConfigFile, 0, configFile, CONFIG_FILE_ENTRIES * sizeof(uint32_t));
   if(error != errNone)
      debugLog("Coludnt write config file, err:%d\n", error);
   
   MemHandleUnlock(configHandle);
   DmReleaseResource(configHandle);
   DmCloseDatabase(configDb);
   
   return 0;
}
