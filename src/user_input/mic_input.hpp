#pragma once

#include <pulse/simple.h>
#include <pulse/error.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <math.h>

class mic_input
{
private:
	pa_simple* s;
	double sample_ms;
	double floor;
	double scale;
	int buf_size;

	bool running;
	std::queue<double> samples;
	std::thread audio_thread;
	std::mutex sample_lock;

	void sample_thread()
	{
		while(running)
		{
			//get audio data
			int error;
			uint8_t buf[buf_size];
			int result = pa_simple_read(s, buf, sizeof(buf), &error);
			if(result < 0)
			{
				std::cout << "Error in audio thread: " << pa_strerror(error) << std::endl;
			}

			//calculate volume in db
			double l_total = 0;
			double r_total = 0;
			for(int i = 0; i < buf_size; i += 4)
			{
				int16_t l = 0;
				int16_t r = 0;

				uint8_t* cur = buf + i;

				//get left channel sample
				l |= *(cur+1);
				l <<= 8;
				l |= *(cur);

				//get right channel sample
				r |= *(cur+3);
				r <<= 8;
				r |= *(cur+2);

				l_total += (l * l);
				r_total += (r * r);
			}

			double RMS_l = sqrt(l_total / ((double)buf_size / 4));
			double RMS_r = sqrt(r_total / ((double)buf_size / 4));

			double average_l = 20 * log10(RMS_l / (double)32768) + 3.0103;
			double average_r = 20 * log10(RMS_r / (double)32768) + 3.0103;

			double scaled_left = (average_l + floor) / scale;
			double scaled_right = (average_r + floor) / scale;

			sample_lock.lock();
			samples.push((scaled_left + scaled_right) / (double)2);
			sample_lock.unlock();
		}
		
	}

	void initialize()
	{
		static const pa_sample_spec ss = {
			.format = PA_SAMPLE_S16LE,
			.rate = 44100,
			.channels = 2
		};

		buf_size = (pa_bytes_per_second(&ss) / (double)1000) * sample_ms;

		//tell pulseaudio what we want the buffer size to be
		static const pa_buffer_attr bb = {
			.maxlength = (uint32_t) - 1,
			.tlength = (uint32_t) - 1,
			.prebuf = (uint32_t) - 1,
			.minreq = (uint32_t) - 1,
			.fragsize = (uint32_t)buf_size
		};

		int error;
		s = pa_simple_new(NULL, "main", PA_STREAM_RECORD, NULL, "vtuber audio input", &ss, NULL, &bb, &error);
		if(!s)
		{
			std::cout << "mic_input: error opening audio stream. Error: " << pa_strerror(error) << std::endl;
			s = NULL;
		}
	}

public:
	mic_input()
	{
		sample_ms = 16.6667;
		floor = 35;
		scale = 20;
		running = true;

		initialize();

		this->audio_thread = std::thread(&mic_input::sample_thread, this);
	}

	~mic_input()
	{
		running = false;
		audio_thread.join();

		//clean up audio object
		if(s)
		{
			pa_simple_free(s);
		}
	}

	//thread safe get of a sample from the queue
	double get()
	{
		sample_lock.lock();
		double temp = 0;
		if(!samples.empty())
		{
			temp = samples.front();
			samples.pop();
		}
		sample_lock.unlock();

		return temp;
	}

	void set_floor(double new_floor)
	{
		floor = new_floor;
	}

	void set_scale(double new_scale)
	{
		scale = new_scale;
	}

	double get_scale()
	{
		return scale;
	}

	double get_floor()
	{
		return floor;
	}

};