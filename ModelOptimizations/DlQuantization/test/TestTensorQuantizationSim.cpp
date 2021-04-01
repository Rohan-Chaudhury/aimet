//==============================================================================
//
//  @@-COPYRIGHT-START-@@
//
//  Copyright (c) 2019, Qualcomm Innovation Center, Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
//  SPDX-License-Identifier: BSD-3-Clause
//
//  @@-COPYRIGHT-END-@@
//
//==============================================================================

#include <gtest/gtest.h>
#include <vector>

#include <TensorQuantizationSim.h>


class TestTensorQuantizationSim : public ::testing::Test
{
};

TEST(TestTensorQuantizationSim, SanityTest)
{
    // Instantiate TensorQuantizationSim
    DlQuantization::TensorQuantizationSim<float> sim;

    // Create a dummy tensor
    const std::vector<float> tensor = {-0.5f, -0.25f, 0, 0.25, 0.5, 0.75};
    std::vector<float> outputTensor(tensor.size());

    uint8_t bw     = 8;
    double min    = -0.46;
    double max    = 0.72;

    sim.quantizeDequantizeTensor(tensor.data(), tensor.size(), outputTensor.data(), min, max, bw,
                                 DlQuantization::RoundingMode::ROUND_NEAREST,  false);

    std::vector<float> expectedOutput = {-0.45811754, -0.2498823, 0.0, 0.2498823, 0.49976459, 0.72188222};

    EXPECT_EQ(outputTensor.size(), expectedOutput.size());

    for (int i = 0; i < outputTensor.size(); i++)
    {
        EXPECT_FLOAT_EQ(outputTensor[i], expectedOutput[i]);
    }
}

TEST(TestTensorQuantizationSim, SanityTestWithGatedMin)
{
    // Instantiate TensorQuantizationSim
    DlQuantization::TensorQuantizationSim<float> sim;

    // Create a dummy tensor
    const std::vector<float> tensor = {-0.5f, -0.25f, 0, 0.25, 0.5, 0.75};
    std::vector<float> outputTensor(tensor.size());

    uint8_t bw     = 8;
    // min is greater than 0, this will be gated at 0.0
    // New range will be 0 to 1.0
    double min    = 0.5;
    double max    = 1.0;

    sim.quantizeDequantizeTensor(tensor.data(), tensor.size(), outputTensor.data(), min, max, bw,
                                 DlQuantization::RoundingMode::ROUND_NEAREST,  false);

    std::vector<float> expectedOutput = {0.0, 0.0, 0.0, 0.25098041,  0.50196081, 0.74901962};

    EXPECT_EQ(outputTensor.size(), expectedOutput.size());

    for (int i = 0; i < outputTensor.size(); i++)
    {
        EXPECT_FLOAT_EQ(outputTensor[i], expectedOutput[i]);
    }
}

TEST(TestTensorQuantizationSim, SanityTestWithGatedMinMaxEqual)
{
    // Instantiate TensorQuantizationSim
    DlQuantization::TensorQuantizationSim<float> sim;

    // Create a dummy tensor
    const std::vector<float> tensor = {-0.5f, -0.25f, 0, 0.25, 0.5, 0.75};
    std::vector<float> outputTensor(tensor.size());

    uint8_t bw     = 8;
    // min max are equal
    // New range will be 0.5 to 0.5+1e-5
    double min    = 0.5;
    double max    = 0.5;

    sim.quantizeDequantizeTensor(tensor.data(), tensor.size(), outputTensor.data(), min, max, bw,
                                 DlQuantization::RoundingMode::ROUND_NEAREST,  false);

    std::vector<float> expectedOutput = {0.0, 0.0, 0.0, 0.25098041,  0.5, 0.5};

    EXPECT_EQ(outputTensor.size(), expectedOutput.size());

    for (int i = 0; i < outputTensor.size(); i++)
    {
        EXPECT_FLOAT_EQ(outputTensor[i], expectedOutput[i]);
    }
}

TEST(TestTensorQuantizationSim, SanityTestWithGatedMax)
{
    // Instantiate TensorQuantizationSim
    DlQuantization::TensorQuantizationSim<float> sim;

    // Create a dummy tensor
    const std::vector<float> tensor = {-0.5f, -0.25f, 0, 0.25, 0.5, 0.75};
    std::vector<float> outputTensor(tensor.size());

    uint8_t bw     = 8;
    // min is greater than 0, this will be gated at 0.0
    // New range will be -0.5 to 0.0
    double min    = -0.5;
    double max    = -0.1;

    sim.quantizeDequantizeTensor(tensor.data(), tensor.size(), outputTensor.data(), min, max, bw,
                                 DlQuantization::RoundingMode::ROUND_NEAREST,  false);

    std::vector<float> expectedOutput = {-0.5, -0.25098041, 0.0, 0.0, 0.0, 0.0};

    EXPECT_EQ(outputTensor.size(), expectedOutput.size());

    for (int i = 0; i < outputTensor.size(); i++)
    {
        EXPECT_FLOAT_EQ(outputTensor[i], expectedOutput[i]);
    }
}

TEST(TestTensorQuantizationSim, SanityTestWithQuantizeOnly)
{
    // Instantiate TensorQuantizationSim
    DlQuantization::TensorQuantizationSim<float> sim;

    // Create a dummy tensor
    const std::vector<float> tensor = {-0.5f, -0.25f, 0, 0.25, 0.5, 0.75};
    std::vector<float> outputTensor(tensor.size());

    uint8_t bw     = 8;
    double min    = -0.46;
    double max    = 0.72;

    sim.quantizeTensor(tensor.data(), tensor.size(), outputTensor.data(), min, max, bw,
                       DlQuantization::RoundingMode::ROUND_NEAREST, false);

    std::vector<float> expectedOutput = {0, 45, 99, 153, 207, 255};

    EXPECT_EQ(outputTensor.size(), expectedOutput.size());

    for (int i = 0; i < outputTensor.size(); i++) {
        EXPECT_FLOAT_EQ(outputTensor[i], expectedOutput[i]);
    }
}
