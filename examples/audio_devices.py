import pyaudio

def list_audio_devices():
    p = pyaudio.PyAudio()
    info = p.get_host_api_info_by_index(0)
    num_devices = info.get('deviceCount')
    
    device_list = []
    for i in range(num_devices):
        device = p.get_device_info_by_host_api_device_index(0, i)
        device_name = device['name']
        device_list.append(device_name)
    
    p.terminate()
    
    return device_list

# Example usage
audio_devices = list_audio_devices()
for i, device in enumerate(audio_devices):
    print(f"Device {i+1}: {device}")
