/*
Demonstrates how to load a sound file and play it back using the low-level API.
The low-level API uses a callback to deliver audio between the application and miniaudio for playback or recording. When
in playback mode, as in this example, the application sends raw audio data to miniaudio which is then played back through
the default playback device as defined by the operating system.
This example uses the `ma_decoder` API to load a sound and play it back. The decoder is entirely decoupled from the
device and can be used independently of it. This example only plays back a single sound file, but it's possible to play
back multiple files by simple loading multiple decoders and mixing them (do not create multiple devices to do this). See
the simple_mixing example for how best to do this.
*/
#include <miniaudio.h>

#include <stdio.h>

struct AudioData {
    bool loop = true;
    ma_decoder decoder;
};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    AudioData* adata = (AudioData*)pDevice->pUserData;
    ma_decoder* pDecoder = &adata->decoder;
    if (pDecoder == NULL) {
        return;
    }
    if (adata->loop) {
        ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, MA_TRUE);
    }
    else
        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}


int main(int argc, char** argv)
{
    ma_result result;
    ma_decoder decoder;
    AudioData adata;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file("./assets/sounds/shotgun.wav", NULL, &adata.decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", "./assets/sounds/XYZ2.ogg");
        return -2;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = adata.decoder.outputFormat;
    deviceConfig.playback.channels = adata.decoder.outputChannels;
    deviceConfig.sampleRate = adata.decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &adata;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    printf("Press Enter to quit...");
    getchar();

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return 0;
}