# Threaded WebGPU improvements

This branch preserves working engine changes for threaded WebGPU workloads.
It builds on David Walter's WebGPU implementation, based on
Godot 4.6.2, at commit `f329e39ce8db7acaa5c9d6628a530fb769969228`.
The changes were developed with OpenAI Codex assistance. This repository is an
independent fork; it is not an upstream pull request or an official release.

## Changes

- **GPU runtime ownership:** `drivers/webgpu/device_thread.h` marshals WebGPU C API
  calls from pthreads to the Emscripten main runtime that owns JavaScript GPU
  objects. CPU mesh generation and shader translation remain on workers. GPU
  submission is serialized; this does not create independent GPU devices per worker.
- **Shader concurrency:** guard the SPIR-V/WGSL cache, make diagnostic counters
  atomic, and prepare shader translations before the RenderingDevice lock.
- **Shader correctness:** use the SPIR-V grammar to distinguish IDs from literal
  operands during resource-array rewriting; preserve Tint array layouts and
  pointer access modes; replace unsupported filtered depth lookups with bilinear
  interpolation of texel loads.
- **Native handles:** expose the underlying WebGPU texture, texture view, and
  device handles for external presentation experiments.
- **Windows builds:** skip the POSIX-only host WGSL precompiler and use runtime
  Tint translation. Keep optimized C++ while reducing the post-link optimization
  and debug-info cost during development.

Application-specific native modules, assets, backend code, and user data are not
part of this branch.

## Build

Install Python, SCons (`python -m pip install scons`), and the Emscripten SDK.
The tested SDK version is **4.0.10**. Activate it and source its environment
before building. From the SDK directory, for example:

```powershell
./emsdk.bat install 4.0.10
./emsdk.bat activate 4.0.10
./emsdk_env.ps1
```

From this repository:

```powershell
python -m SCons platform=web target=template_release webgpu=yes opengl3=no threads=yes dlink_enabled=no optimize=speed lto=none initial_memory=512 debug_symbols=yes -j12
```

The template archive is `bin/godot.web.template_release.wasm32.zip`. Select it as
the custom release template in a compatible Godot 4.6.2 project's Web export
preset. Enable thread support and disable extension support for this static
template. The prototypes used 20 Emscripten workers and 12 Godot workers.
An official Godot 4.6.2 editor was used to package project resources; this is not
a claim that the upstream editor provides a native WebGPU editing workflow.

Serve over HTTPS or localhost with these headers:

```text
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

The browser must support WebGPU, SharedArrayBuffer, and cross-origin isolation.
Use compatible policies for every subresource. The tested applications reject
WebGL fallback explicitly; an ordinary web server without these headers is not
sufficient for the threaded template.

## Validation and limits

The checked-in [historical validation record](webgpu_notes/threaded/validation.json)
comes from the same engine changes running an isolated workload in the original
application harness on Windows, Chrome, and an NVIDIA RTX 5090. It records 16 mesh
tasks, 524,502 primitives, multiple mesh worker IDs, a real WebGPU context, no
WebGL contexts, cross-origin isolation, and no recorded GPU errors. The associated
console analysis found 12 mesh workers and 12 overlapping CPU shader translations.
That console log and the application harness remain in the application workspace;
the JSON alone does not establish shader-translation overlap.

**Cold GPU pipeline creation still stalls.** The recorded worst browser frame
was approximately **6,672 ms**. Multithreaded CPU shader preparation does not make
synchronous pipeline creation asynchronous or guarantee 60 FPS. Full application
room tests also did not meet every sub-100 ms loading target.

This snapshot retains the tested engine source unchanged. Publishing it does not
constitute a fresh clean build, a cross-browser certification, a physical-headset
test, or a performance comparison with Three.js. The minimal engine-only build
does not include the application modules used in the historical test.

## Attribution and licensing

Retain [LICENSE.txt](LICENSE.txt), [COPYRIGHT.txt](COPYRIGHT.txt), and all bundled
third-party license notices. Godot is MIT licensed. David Walter's WebGPU backend
is the foundation of this work; the upstream README and documentation remain in
the repository. The changes in this branch are distributed under the repository's
existing applicable licenses, including those of modified third-party files.
