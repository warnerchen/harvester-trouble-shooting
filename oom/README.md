# Out Of Memory (OOM) Troubleshooting

Manually trigger VM OOM using `memhold` and `memtest`.

## Quick Start

Requirements:
1. Guest OS: gcc installed
2. virt-launcher Pod: gcc installed

Guest OS:

```bash
gcc memtest.c -o memtest
./memtest
```

virt-launcher Pod:

```Dockerfile
FROM <virt-launcher image>

COPY memhold.c /tmp

RUN zypper install -y gcc \
    && gcc /tmp/memhold.c -o /tmp/memhold
```

```bash
kubectl exec -it <virt-launcher pod> -- /tmp/memhold 100
```

First, run `memhold` in the virt-launcher pod, then execute `memtest` in the Guest OS. This will trigger an OOM event, and you can observe the corresponding kernel logs.

```log
...
[Wed Mar 26 02:14:58 2025] CPU 0/KVM invoked oom-killer: gfp_mask=0xcc0(GFP_KERNEL), order=0, oom_score_adj=972
...
[Wed Mar 26 02:14:58 2025] Memory cgroup out of memory: Killed process 24324 (qemu-system-x86) total-vm:1642784kB, anon-rss:610808kB, file-rss:24768kB, shmem-rss:0kB, UID:107 pgtables:1704kB oom_score_adj:972
...
```