#ifndef PROCESSOR_ENGINE_H

#define PROCESSOR_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ProcessorStruct.h"
#include "ProcessorCommands.h"
#include "../common/commands_info.h"

const char*     GetErrorString    (CMD_STATUS cmd_status);

CMD_STATUS      RunCommand        (Processor* processor);

int             RunProcessor      (Processor* processor);

#endif