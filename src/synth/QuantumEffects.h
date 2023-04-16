#pragma once

#include <complex>
#include <vector>
#include <cmath>
#include <array>
#include <queue>

namespace cw::synth {

using cVector = std::vector<std::complex<float>>;
using cMatrix = std::vector<cVector>;

struct Spin3 {
	const std::complex<float> imag{ 0, 1 };
	const cMatrix S_x = { 
		{0, std::sqrt(3), 0, 0},
		{std::sqrt(3), 0, 2, 0},
		{0, 2, 0, std::sqrt(3)},
		{0, 0, std::sqrt(3), 0}
	};
	const cMatrix S_y = {
		{0, (0, -std::sqrt(3)), 0, 0},
		{(0, std::sqrt(3)), 0, (0, -2), 0},
		{0, (0, 2), 0, (0, -std::sqrt(3))},
		{0, 0, (0, std::sqrt(3)), 0}
	};
	const cMatrix S_z = {
		{3, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, -1, 0},
		{0, 0, 0, -3}
	};
};

class Spin3Rotation {
	public:
		Spin3Rotation();
		// Clears the buffer - should always be called when a note stops playing. 
		void clearBuffer();
		/*
		* This method will do the actual transformation of the input data. It does a complex spin rotation on the input
		* vectors, depending on the angles theta and phi. The method expects two vectors: one for the left and one for
		* the right channel. The left channel is treated as real, the right channel as imaginary part. For the ouput,
		* it is vice versa: Real to left, imaginary to right. 
		*/
		std::array<std::vector<float>, 2> spinRotate(const std::array<std::vector<float>, 2>& inVec);

		void setTheta(float theta) { this->theta = theta; }
		void setPhi(float phi) { this->phi = phi; }

	private:
		float theta{ 0 }; // in radians
		float phi{ 0 }; // in radians
		Spin3 spins{};
		/*
		* The buffer within which possible unprocessed samples from the previous chunk are saved. It is a queue, so each
		* time there are remaining samples from the last chunk to be processed, these will be the first to be picked.
		*/
		std::queue<std::complex<float>> buffer;
		/*
		* Convenience method to multiply a complex matrix with a complex vector and output the result. The matrix is
		* expected to be square, and thus fitting the length of the vector. 
		*/
		cVector matMult(const cMatrix& matrix, const cVector& inVec, float factor);
		/*
		* Convenience method to add two complex vectors.
		*/
		cVector cVecAdd(const cVector&, const cVector&);

};

} // namespace cw::synth