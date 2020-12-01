/**
 * @file mha.h
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

#include <array>
#include <vector>
#include <functional>
#include <experimental/optional>

#include <torch/torch.h>

namespace mha{

  using Scalar = torch::Scalar;
  using Tensor = torch::Tensor;

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

  /**
   * @brief Generate random numbers in that range
   *
   * @param uplimit: upper limit
   * @param downlimit: lower limit
   * @param size: Output Tensor size
   * @param options: Tensor settings
   * @return Tensor
   */
  Tensor rand(const Tensor& uplimit, const Tensor& downlimit, at::IntArrayRef size, const at::TensorOptions &options){
    // 0 ~ 1
    auto scale = torch::rand(size, options);

    // 0 start
    const auto diff = uplimit - downlimit;

    scale *= diff;
    scale += downlimit;
    return scale;
  }

  /**
   * @brief Store non-zero index for each row in an array
   *
   * @param condition: A two-dimensional array
   * @return std::vector<Tensor>
   */
  std::vector<Tensor> nonzero(const Tensor& condition){
      auto vIndex = std::vector<torch::Tensor>(condition.sizes()[0]);
      for(auto i = 0; i < vIndex.size(); ++i) {
        vIndex[i] = condition[i].nonzero();
      }
      return vIndex;
  }


};