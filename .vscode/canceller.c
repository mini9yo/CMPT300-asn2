#include "inputThread.h"
#include "sendThread.h"
#include "receiveThread.h"
#include "printThread.h"

void receiveCancel()
{
    cancelReceive();
    cancelPrint();
}

void sendCancel()
{
    cancelInput();
    cancelSend();
}