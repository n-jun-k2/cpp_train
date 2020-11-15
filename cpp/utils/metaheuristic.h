/**
 * @file metaheuristic.h
 * 
 * @author jun kawakami 
 * @brief 
 * @version 0.1.0
 * @date 2020-11-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

#include <functional>
#include <torch/torch.h>

namespace metaheuristic{

	const auto CPU_OPTIONS = torch::TensorOptions()
										.dtype(torch::kFloat32)
										.layout(torch::kStrided)
										.device(torch::kCPU)
										.requires_grad(false);

	const auto GPU_OPTIONS = torch::TensorOptions()
										.dtype(torch::kFloat32)
										.layout(torch::kStrided)
										.device(torch::kCUDA)
										.requires_grad(false);

	/**
	 * @brief Get the Available Option object
	 * 
	 * @return torch::TensorOptions GPU_OPTIONS or CPU_OPTIONS
	 */
	torch::TensorOptions getAvailableOption(){
		return torch::cuda::is_available() ? GPU_OPTIONS : CPU_OPTIONS;
	}

	

};