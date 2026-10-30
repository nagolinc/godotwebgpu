// WebGPU JS handles belong to the device-owning runtime, not each pthread.
// Only API submission crosses threads: GLSL/SPIR-V/Tint work stays on workers.
#pragma once
#include <emscripten/threading.h>
#include <emscripten/threading_legacy.h>
#include <type_traits>

template <typename F> auto school_device_call(F &&call) -> decltype(call()) {
	if (emscripten_is_main_runtime_thread()) {
		return call();
	}
	using Result = decltype(call());
	if constexpr (std::is_void_v<Result>) {
		auto thunk = +[](void *ptr) { (*static_cast<std::remove_reference_t<F> *>(ptr))(); };
		emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, thunk, &call);
	} else {
		Result result{};
		auto invoke = [&]() { result = call(); };
		auto thunk = +[](void *ptr) { (*static_cast<decltype(invoke) *>(ptr))(); };
		emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, thunk, &invoke);
		return result;
	}
}

#define SCHOOL_DEVICE_CALL(name, ...) school_device_call([&]() { return (name)(__VA_ARGS__); })
#define wgpuBindGroupLayoutRelease(...) SCHOOL_DEVICE_CALL(wgpuBindGroupLayoutRelease, __VA_ARGS__)
#define wgpuBindGroupRelease(...) SCHOOL_DEVICE_CALL(wgpuBindGroupRelease, __VA_ARGS__)
#define wgpuBindGroupSetLabel(...) SCHOOL_DEVICE_CALL(wgpuBindGroupSetLabel, __VA_ARGS__)
#define wgpuBufferGetConstMappedRange(...) SCHOOL_DEVICE_CALL(wgpuBufferGetConstMappedRange, __VA_ARGS__)
#define wgpuBufferGetMapState(...) SCHOOL_DEVICE_CALL(wgpuBufferGetMapState, __VA_ARGS__)
#define wgpuBufferGetMappedRange(...) SCHOOL_DEVICE_CALL(wgpuBufferGetMappedRange, __VA_ARGS__)
#define wgpuBufferMapAsync(...) SCHOOL_DEVICE_CALL(wgpuBufferMapAsync, __VA_ARGS__)
#define wgpuBufferRelease(...) SCHOOL_DEVICE_CALL(wgpuBufferRelease, __VA_ARGS__)
#define wgpuBufferSetLabel(...) SCHOOL_DEVICE_CALL(wgpuBufferSetLabel, __VA_ARGS__)
#define wgpuBufferUnmap(...) SCHOOL_DEVICE_CALL(wgpuBufferUnmap, __VA_ARGS__)
#define wgpuCommandBufferRelease(...) SCHOOL_DEVICE_CALL(wgpuCommandBufferRelease, __VA_ARGS__)
#define wgpuCommandEncoderBeginComputePass(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderBeginComputePass, __VA_ARGS__)
#define wgpuCommandEncoderBeginRenderPass(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderBeginRenderPass, __VA_ARGS__)
#define wgpuCommandEncoderClearBuffer(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderClearBuffer, __VA_ARGS__)
#define wgpuCommandEncoderCopyBufferToBuffer(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderCopyBufferToBuffer, __VA_ARGS__)
#define wgpuCommandEncoderCopyBufferToTexture(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderCopyBufferToTexture, __VA_ARGS__)
#define wgpuCommandEncoderCopyTextureToBuffer(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderCopyTextureToBuffer, __VA_ARGS__)
#define wgpuCommandEncoderCopyTextureToTexture(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderCopyTextureToTexture, __VA_ARGS__)
#define wgpuCommandEncoderFinish(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderFinish, __VA_ARGS__)
#define wgpuCommandEncoderInsertDebugMarker(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderInsertDebugMarker, __VA_ARGS__)
#define wgpuCommandEncoderPopDebugGroup(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderPopDebugGroup, __VA_ARGS__)
#define wgpuCommandEncoderPushDebugGroup(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderPushDebugGroup, __VA_ARGS__)
#define wgpuCommandEncoderRelease(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderRelease, __VA_ARGS__)
#define wgpuCommandEncoderResolveQuerySet(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderResolveQuerySet, __VA_ARGS__)
#define wgpuCommandEncoderWriteTimestamp(...) SCHOOL_DEVICE_CALL(wgpuCommandEncoderWriteTimestamp, __VA_ARGS__)
#define wgpuComputePassEncoderDispatchWorkgroups(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderDispatchWorkgroups, __VA_ARGS__)
#define wgpuComputePassEncoderDispatchWorkgroupsIndirect(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderDispatchWorkgroupsIndirect, __VA_ARGS__)
#define wgpuComputePassEncoderEnd(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderEnd, __VA_ARGS__)
#define wgpuComputePassEncoderInsertDebugMarker(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderInsertDebugMarker, __VA_ARGS__)
#define wgpuComputePassEncoderPopDebugGroup(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderPopDebugGroup, __VA_ARGS__)
#define wgpuComputePassEncoderPushDebugGroup(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderPushDebugGroup, __VA_ARGS__)
#define wgpuComputePassEncoderRelease(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderRelease, __VA_ARGS__)
#define wgpuComputePassEncoderSetBindGroup(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderSetBindGroup, __VA_ARGS__)
#define wgpuComputePassEncoderSetPipeline(...) SCHOOL_DEVICE_CALL(wgpuComputePassEncoderSetPipeline, __VA_ARGS__)
#define wgpuComputePipelineRelease(...) SCHOOL_DEVICE_CALL(wgpuComputePipelineRelease, __VA_ARGS__)
#define wgpuComputePipelineSetLabel(...) SCHOOL_DEVICE_CALL(wgpuComputePipelineSetLabel, __VA_ARGS__)
#define wgpuDeviceCreateBindGroup(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateBindGroup, __VA_ARGS__)
#define wgpuDeviceCreateBindGroupLayout(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateBindGroupLayout, __VA_ARGS__)
#define wgpuDeviceCreateBuffer(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateBuffer, __VA_ARGS__)
#define wgpuDeviceCreateCommandEncoder(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateCommandEncoder, __VA_ARGS__)
#define wgpuDeviceCreateComputePipeline(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateComputePipeline, __VA_ARGS__)
#define wgpuDeviceCreatePipelineLayout(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreatePipelineLayout, __VA_ARGS__)
#define wgpuDeviceCreateQuerySet(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateQuerySet, __VA_ARGS__)
#define wgpuDeviceCreateRenderPipeline(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateRenderPipeline, __VA_ARGS__)
#define wgpuDeviceCreateSampler(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateSampler, __VA_ARGS__)
#define wgpuDeviceCreateShaderModule(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateShaderModule, __VA_ARGS__)
#define wgpuDeviceCreateTexture(...) SCHOOL_DEVICE_CALL(wgpuDeviceCreateTexture, __VA_ARGS__)
#define wgpuDeviceGetLimits(...) SCHOOL_DEVICE_CALL(wgpuDeviceGetLimits, __VA_ARGS__)
#define wgpuDeviceHasFeature(...) SCHOOL_DEVICE_CALL(wgpuDeviceHasFeature, __VA_ARGS__)
#define wgpuInstanceProcessEvents(...) SCHOOL_DEVICE_CALL(wgpuInstanceProcessEvents, __VA_ARGS__)
#define wgpuPipelineLayoutRelease(...) SCHOOL_DEVICE_CALL(wgpuPipelineLayoutRelease, __VA_ARGS__)
#define wgpuQuerySetRelease(...) SCHOOL_DEVICE_CALL(wgpuQuerySetRelease, __VA_ARGS__)
#define wgpuQueueOnSubmittedWorkDone(...) SCHOOL_DEVICE_CALL(wgpuQueueOnSubmittedWorkDone, __VA_ARGS__)
#define wgpuQueueSubmit(...) SCHOOL_DEVICE_CALL(wgpuQueueSubmit, __VA_ARGS__)
#define wgpuQueueWriteBuffer(...) SCHOOL_DEVICE_CALL(wgpuQueueWriteBuffer, __VA_ARGS__)
#define wgpuQueueWriteTexture(...) SCHOOL_DEVICE_CALL(wgpuQueueWriteTexture, __VA_ARGS__)
#define wgpuRenderPassEncoderDraw(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderDraw, __VA_ARGS__)
#define wgpuRenderPassEncoderDrawIndexed(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderDrawIndexed, __VA_ARGS__)
#define wgpuRenderPassEncoderDrawIndexedIndirect(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderDrawIndexedIndirect, __VA_ARGS__)
#define wgpuRenderPassEncoderDrawIndirect(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderDrawIndirect, __VA_ARGS__)
#define wgpuRenderPassEncoderEnd(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderEnd, __VA_ARGS__)
#define wgpuRenderPassEncoderInsertDebugMarker(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderInsertDebugMarker, __VA_ARGS__)
#define wgpuRenderPassEncoderPopDebugGroup(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderPopDebugGroup, __VA_ARGS__)
#define wgpuRenderPassEncoderPushDebugGroup(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderPushDebugGroup, __VA_ARGS__)
#define wgpuRenderPassEncoderRelease(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderRelease, __VA_ARGS__)
#define wgpuRenderPassEncoderSetBindGroup(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetBindGroup, __VA_ARGS__)
#define wgpuRenderPassEncoderSetBlendConstant(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetBlendConstant, __VA_ARGS__)
#define wgpuRenderPassEncoderSetIndexBuffer(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetIndexBuffer, __VA_ARGS__)
#define wgpuRenderPassEncoderSetPipeline(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetPipeline, __VA_ARGS__)
#define wgpuRenderPassEncoderSetScissorRect(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetScissorRect, __VA_ARGS__)
#define wgpuRenderPassEncoderSetStencilReference(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetStencilReference, __VA_ARGS__)
#define wgpuRenderPassEncoderSetVertexBuffer(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetVertexBuffer, __VA_ARGS__)
#define wgpuRenderPassEncoderSetViewport(...) SCHOOL_DEVICE_CALL(wgpuRenderPassEncoderSetViewport, __VA_ARGS__)
#define wgpuRenderPipelineRelease(...) SCHOOL_DEVICE_CALL(wgpuRenderPipelineRelease, __VA_ARGS__)
#define wgpuRenderPipelineSetLabel(...) SCHOOL_DEVICE_CALL(wgpuRenderPipelineSetLabel, __VA_ARGS__)
#define wgpuSamplerRelease(...) SCHOOL_DEVICE_CALL(wgpuSamplerRelease, __VA_ARGS__)
#define wgpuSamplerSetLabel(...) SCHOOL_DEVICE_CALL(wgpuSamplerSetLabel, __VA_ARGS__)
#define wgpuShaderModuleRelease(...) SCHOOL_DEVICE_CALL(wgpuShaderModuleRelease, __VA_ARGS__)
#define wgpuShaderModuleSetLabel(...) SCHOOL_DEVICE_CALL(wgpuShaderModuleSetLabel, __VA_ARGS__)
#define wgpuSurfaceConfigure(...) SCHOOL_DEVICE_CALL(wgpuSurfaceConfigure, __VA_ARGS__)
#define wgpuSurfaceGetCurrentTexture(...) SCHOOL_DEVICE_CALL(wgpuSurfaceGetCurrentTexture, __VA_ARGS__)
#define wgpuSurfacePresent(...) SCHOOL_DEVICE_CALL(wgpuSurfacePresent, __VA_ARGS__)
#define wgpuSurfaceUnconfigure(...) SCHOOL_DEVICE_CALL(wgpuSurfaceUnconfigure, __VA_ARGS__)
#define wgpuTextureCreateView(...) SCHOOL_DEVICE_CALL(wgpuTextureCreateView, __VA_ARGS__)
#define wgpuTextureGetHeight(...) SCHOOL_DEVICE_CALL(wgpuTextureGetHeight, __VA_ARGS__)
#define wgpuTextureGetWidth(...) SCHOOL_DEVICE_CALL(wgpuTextureGetWidth, __VA_ARGS__)
#define wgpuTextureRelease(...) SCHOOL_DEVICE_CALL(wgpuTextureRelease, __VA_ARGS__)
#define wgpuTextureSetLabel(...) SCHOOL_DEVICE_CALL(wgpuTextureSetLabel, __VA_ARGS__)
#define wgpuTextureViewRelease(...) SCHOOL_DEVICE_CALL(wgpuTextureViewRelease, __VA_ARGS__)
