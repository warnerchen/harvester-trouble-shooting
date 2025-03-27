# KubeVirt Pod Out Of Memory (OOM) Troubleshooting

If a KubeVirt-related pod encounters an OOM event, we can use cluster-profiler to dump the pprof files.

Reference: https://kubevirt.io/user-guide/debug_virt_stack/debug/#kubevirt-pprof-profiler

## Quick Start

### Harvester v1.3.2

```bash
# Prefer the Harvester kubeconfig file and store it together with the cluster-profiler.
# Start a temporary container.
docker run --rm -it -v /<path-to-your-folder>:/go/debug --entrypoint=sh --net host golang:1.19-alpine

# Dump the pprof files.
./cluster-profiler --kubeconfig rke2.yaml --namespace harvester-system --cmd start
2025/03/27 02:15:23 SUCCESS: started cpu profiling KubeVirt control plane

./cluster-profiler --kubeconfig rke2.yaml --namespace harvester-system --cmd dump --l "kubevirt.io=virt-handler"
Fetching in progress. Downloaded so far: 0
SUCCESS: Dumped PProf 1 results for KubeVirt control plane to [cluster-profiler-results]

./cluster-profiler --kubeconfig rke2.yaml --namespace harvester-system --cmd stop
2025/03/27 02:15:32 SUCCESS: stopped cpu profiling KubeVirt control plane
```
