#include "LogStorage.h"

LogStorage::LogStorage(SdCardService &sdCardService) : sdCardService(sdCardService) {
}

void LogStorage::WriteLog(String log) {
    if (!sdCardService.IsReady())
        return;

    File file = sdCardService.GetFileSystem().open("/LOG.txt", FILE_APPEND);
    if (file) {
        file.println(log);

        // Close the file
        file.close();
    }
}
