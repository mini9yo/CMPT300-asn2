int main(int argc, char** args)
{
    // Intialize pthreads
    input_init();
    send_init();
    receive_init();
    print_init();

    // Cleanup pthreads
    input_waitForShutdown();
    send_waitForShutdown();
    receive_waitForShutdown();
    print_waitForShutdown();

    return 0;
}