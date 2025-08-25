# KubeVirt Pod Out Of Memory (OOM) Troubleshooting

If a KubeVirt related pod encounters an OOM event, we can use `cluster-profiler` to dump the pprof files.

Reference: https://kubevirt.io/user-guide/debug_virt_stack/debug/#kubevirt-pprof-profiler

## Quick Start

### Requirements

KubeVirt requires the `ClusterProfiler` **featureGate** to be enabled. In **Harvester**, this can be configured by setting `.spec.values.kubevirt.spec.configuration.developerConfiguration.featureGates` in `managedcharts.management.cattle.io`.

You can use this command to obtain the currently enabled featureGates for KubeVirt. The default configuration for HV is as follows:

```bash
kubectl -n harvester-system get kubevirts.kubevirt.io kubevirt -ojsonpath={.spec.configuration.developerConfiguration} | jq
{
  "featureGates": [
    "LiveMigration",
    "HotplugVolumes",
    "HostDevices",
    "GPU",
    "CPUManager"
  ]
}
```

Then modify `managedcharts.management.cattle.io harvester` and add the `ClusterProfiler` featureGate on top of that.

```bash
kubectl -n fleet-local edit managedcharts.management.cattle.io harvester
```

```yaml
...
spec:
  values:
    kubevirt:
      spec:
        configuration:
          developerConfiguration:
            featureGates:
            - ClusterProfiler
            - LiveMigration
            - HotplugVolumes
            - HostDevices
            - GPU
            - CPUManager
...
```

### Build

KubeVirt does not officially provide pre-built tools by default, so they need to be built manually.

```bash
git clone https://github.com/kubevirt/kubevirt.git

cd kubevirt

# The KubeVirt version needs to correspond to the KubeVirt version used by Harvester.
git checkout v1.4.0

# The Go version needs to correspond to the Go version used by KubeVirt. You can confirm the Go version in kubevirt/go.mod.
docker run --rm -it -v $(pwd)/kubevirt:/kubevirt golang:1.22-alpine sh

cd /kubevirt/tools/cluster-profiler

# Once the build is complete, you can then obtain the built cluster-profiler from the original KubeVirt directory.
go build
```

> You can also use the prebuilt `cluster-profiler` provided in this project. Please note that it is only supported on AMD64 environments.

### Usage

```bash
# Prefer the Harvester kubeconfig file and store it together with the cluster-profiler.
# Start a temporary container. The Go version needs to correspond to the Go version used by KubeVirt. You can confirm KubeVirt version on the Harvester release page.
# By default, HV nodes do not have nerdctl tools installed, and you need to install them manually.
nerdctl run --rm -it -v /<path-to-your-folder>:/go/debug --net host golang:1.22-alpine sh

cd /go/debug

# Dump the pprof files.
./cluster-profiler --kubeconfig rke2.yaml --namespace harvester-system --cmd start
2025/03/27 02:15:23 SUCCESS: started cpu profiling KubeVirt control plane

./cluster-profiler --kubeconfig rke2.yaml --namespace harvester-system --cmd dump --l "kubevirt.io=virt-handler"
Fetching in progress. Downloaded so far: 0
SUCCESS: Dumped PProf 1 results for KubeVirt control plane to [cluster-profiler-results]

./cluster-profiler --kubeconfig rke2.yaml --namespace harvester-system --cmd stop
2025/03/27 02:15:32 SUCCESS: stopped cpu profiling KubeVirt control plane
```
