#pragma once

namespace RaytracingRenderer {

	class Accumulator {
	private:
		uint sample_amount = 0;
		// Accumulates energy.
		float3 accumulated_energy[SCRHEIGHT][SCRWIDTH];
	public:
		inline float3 accumulateEnergy(float3& energy, int x, int y) {
			float3 total_energy = accumulated_energy[y][x];
			// If this is the first sample, we just return the current energy.
			if (sample_amount == 1) {
				accumulated_energy[y][x] = energy;
				return energy;
			}

			// Calculate the new average.
			float3 new_energy = total_energy + ((energy - total_energy) / static_cast<float>(sample_amount));
			accumulated_energy[y][x] = new_energy;

			return new_energy;
		}
		inline void incrementAccumulation() { sample_amount += 1; }
		inline Accumulator() {}
		inline void resetAccumulator() {
			sample_amount = 0;
			memset(&accumulated_energy, 0, (size_t)(SCRHEIGHT * SCRWIDTH) * sizeof(float3));
		}
	};
}