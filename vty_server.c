#include <vty_comm.h>
#include <vty.h>
#include <thread.h>


#define BGP_VTY_PORT 2222

/* Process ID saved for use by init system */
static const char *pid_file = PATH_BGPD_PID;

/* Manually specified configuration file name.  */
char *config_file = NULL;

/* VTY port number and address.  */
int vty_port = BGP_VTY_PORT;
char *vty_addr = NULL;

/* privileges */
static zebra_capabilities_t _caps_p [] =
{
    ZCAP_BIND,
    ZCAP_NET_RAW,
    ZCAP_NET_ADMIN,
};


struct zebra_privs_t vtyserver_privs = {
    #if defined(QUAGGA_USER)
    .user = QUAGGA_USER,
    #endif
    #if defined QUAGGA_GROUP
    .group = QUAGGA_GROUP,
    #endif
    #ifdef VTY_GROUP
    .vty_group = VTY_GROUP,
    #endif
    .caps_p = _caps_p,
    .cap_num_p = sizeof (_caps_p) / sizeof (*_caps_p),
    .cap_num_i = 0
};

void
reload ()
{
    zlog_debug ("Reload");
    /* FIXME: Clean up func call here */
    vty_reset ();
    (void) isisd_privs.change (ZPRIVS_RAISE);
    execve (_progpath, _argv, _envp);
    zlog_err ("Reload failed: cannot exec %s: %s", _progpath,
    safe_strerror (errno));
}

static void
terminate (int i)
{
    exit (i);
}



/* SIGHUP handler. */
static void
sighup (void)
{
  zlog_info ("SIGHUP received");

  /* Reload of config file. */
  ;
}

/* SIGINT handler. */
static void
sigint (void)
{
  zlog_notice ("Terminating on signal");

/*
 *  if (!retain_mode)
 *    rib_close ();
 *#ifdef HAVE_IRDP
 *  irdp_finish();
 *#endif
 */

  exit (0);
}

/* SIGUSR1 handler. */
static void
sigusr1 (void)
{
  zlog_rotate (NULL);
}

struct quagga_signal_t zebra_signals[] =
{
  {
    .signal = SIGHUP,
    .handler = &sighup,
  },
  {
    .signal = SIGUSR1,
    .handler = &sigusr1,
  },
  {
    .signal = SIGINT,
    .handler = &sigint,
  },
  {
    .signal = SIGTERM,
    .handler = &sigint,
  },
};

int
main(int argc, char **argv)
{

    /* thread master */
    master = thread_master_create ();

    /* random seed from time */
    srandom (time (NULL));

    /*
    * initializations
    */
    zprivs_init (&vtyserver_privs);

    /* Vty related initialize. */
    signal_init (master, array_size(zebra_signals), zebra_signals);

    cmd_init(1);
    vty_init(master);
    memory_init();

    /* BGP related initialization.  */
    bgp_init ();

    vty_read_config(config_file, config_default);

    /* Process ID file creation. */
    pid_output (pid_file);

    vty_serv_sock(vty_addr, vty_port ,BGP_VTYSH_PATH);

    thread_main(master);
}
