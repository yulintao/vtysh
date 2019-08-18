#include <vty.h>
#include <thread.h>

int
main(int argc, char **argv)
{
    cmd_init(1);
    vty_init(bm->master);
    memory_init();



    vty_read_config(config_file, config_default);

    vty_serv_sock(vty_addr, vty_port ,BGP_VTYSH_PATH);

    thread_main(bm->master);
}
