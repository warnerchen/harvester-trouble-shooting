# VM Out Of Memory (OOM) Troubleshooting

Manually trigger VM OOM using `memhold` and `memtest`.

## Quick Start

Requirements:
1. Guest OS: gcc installed
2. virt-launcher Pod: gcc installed

## Guest OS

It is recommended to use the same version of the image to build the debug tool, rather than building and installing it directly inside the Guest OS.

```Dockerfile
FROM <Guest OS Image>

COPY memtest.c /tmp

# Adjust the command according to the Guest OS
RUN apt -y install gcc \
    && gcc /tmp/memtest.c -o /tmp/memtest
```

Get `memtest` from temporary container.

```bash
CID=$(docker create <Guest OS Debug Tool Installed Image>)

docker cp "$CID":/tmp/memtest ./memtest

docker rm $CID
```

Upload `memtest` to the Guest OS.

```bash
scp memtest root@xxx:/tmp

ssh root@xxx

./tmp/memtest
```

## virt-launcher Pod

```Dockerfile
FROM <Virt Launcher Image>

COPY memhold.c /tmp

RUN zypper install -y gcc \
    && gcc /tmp/memhold.c -o /tmp/memhold
```

When building a custom virt-launcher image, make sure the image name and tag are consistent with the original image. After building, you can overwrite the corresponding image in the HV cluster and apply the changes by restarting the VM. To avoid unexpected issues, it is recommended to back up the original virt-launcher image before overwriting it.

```bash
ctr -n k8s.io image import <Virt Launcher Debug Tool Installed Image>
```

```bash
kubectl exec -it <virt-launcher Pod> -- /tmp/memhold 100
```

First, run `memhold` in the virt-launcher Pod, then execute `memtest` in the Guest OS. This will trigger an OOM event, and you can observe the corresponding kernel logs.

```log
...
[Wed Mar 26 02:14:58 2025] CPU 0/KVM invoked oom-killer: gfp_mask=0xcc0(GFP_KERNEL), order=0, oom_score_adj=972
...
[Wed Mar 26 02:14:58 2025] Memory cgroup out of memory: Killed process 24324 (qemu-system-x86) total-vm:1642784kB, anon-rss:610808kB, file-rss:24768kB, shmem-rss:0kB, UID:107 pgtables:1704kB oom_score_adj:972
...
```