#pragma once

#include<string>
#include <functional>

namespace ParaEngine
{
    struct PlayerState {
    public:
        int32_t index() const { return index_; };
        //Playlist* medias() const { return medias_.get(); };
        bool is_playing() const { return is_playing_; };
        bool is_valid() const { return is_valid_; };
        bool is_seekable() const { return is_seekable_; };
        bool is_completed() const { return is_completed_; };
        int32_t position() const { return position_; };
        int32_t duration() const { return duration_; };
        float volume() const { return volume_; }
        float rate() const { return rate_; }
        bool is_playlist() const { return is_playlist_; };
        bool is_started() const { return is_started_; };

        void set_index(int32_t value) { index_ = value; };
        /*void set_medias(std::unique_ptr<Playlist> value) {
            medias_.reset(value.release());
        };*/
        void set_is_playing(bool value) { is_playing_ = value; };
        void set_is_valid(bool value) { is_valid_ = value; };
        void set_is_seekable(bool value) { is_seekable_ = value; };
        void set_is_completed(bool value) { is_completed_ = value; };
        void set_position(int32_t value) { position_ = value; };
        void set_duration(int32_t value) { duration_ = value; };
        void set_volume(float value) { volume_ = value; }
        void set_rate(float value) { rate_ = value; }
        void set_is_playlist(bool value) { is_playlist_ = value; };
        void set_is_started(bool value) { is_started_ = value; };

        void Reset() {
            //medias_->medias().clear();
            index_ = 0;
            is_playing_ = false;
            is_valid_ = true;
            is_seekable_ = true;
            is_completed_ = false;
            position_ = 0;
            duration_ = 0;
            is_started_ = false;
        }

    private:
        int32_t index_ = 0;
        //std::unique_ptr<Playlist> medias_ =
        //    std::make_unique<Playlist>(std::vector<std::shared_ptr<Media>>{});
        bool is_playing_ = false;
        bool is_valid_ = true;
        bool is_seekable_ = true;
        bool is_completed_ = false;
        int32_t position_ = 0;
        int32_t duration_ = 0;
        float volume_ = 1.0;
        float rate_ = 1.0;
        bool is_playlist_ = false;
        bool is_started_ = false;
    };

    struct DartDeviceList {
        struct Device {
            const char* name;
            const char* id;
            explicit Device(const char* name, const char* id) : name(name), id(id) {}
        };

        int32_t size;
        const Device* device_infos;
    };

    struct DartEqualizer {
        int32_t id;
        float pre_amp;
        const float* bands;
        const float* amps;
        int32_t size;
    };

    class IVideoPlayers
    {
    protected:
        std::function<void(int32_t id, PlayerState* state)> onPlayPauseStop = nullptr;
        std::function<void(int32_t id, PlayerState* state)> onPosition = nullptr;
        std::function<void(int32_t id, PlayerState* state)> onComplete = nullptr;
        std::function<void(int32_t id, PlayerState* state)> onVolume = nullptr;
        std::function<void(int32_t id, PlayerState* state)> onRate = nullptr;
        std::function<void(int32_t id, PlayerState* state)> onOpen = nullptr;
        std::function<void(int32_t id, int32_t video_width, int32_t video_height)> onVideoDimensions = nullptr;

        std::function<void(int32_t id, float buffering)> onBuffering = nullptr;
        std::function<void(int32_t id, const char* error)> onError = nullptr;
    public:
        void setCallback_onPlayPauseStop(std::function<void(int32_t id, PlayerState* state)> callback) { onPlayPauseStop = callback; };
        void setCallback_onPosition(std::function<void(int32_t id, PlayerState* state)> callback) { onPosition = callback; };
        void setCallback_onComplete(std::function<void(int32_t id, PlayerState* state)> callback) { onComplete = callback; };
        void setCallback_onVolume(std::function<void(int32_t id, PlayerState* state)> callback) { onVolume = callback; };
        void setCallback_onRate(std::function<void(int32_t id, PlayerState* state)> callback) { onRate = callback; };
        void setCallback_onOpen(std::function<void(int32_t id, PlayerState* state)> callback) { onOpen = callback; };
        void setCallback_onVideoDimensions(std::function<void(int32_t id, int32_t video_width, int32_t video_height)> callback) { onVideoDimensions = callback; };
        void setCallback_onBuffering(std::function<void(int32_t id, float buffering)> callback) { onBuffering = callback; };
        void setCallback_onError(std::function<void(int32_t id, const char* error)> callback) { onError = callback; };

        virtual void setCallback_onVideoFrame(int32_t id, std::function<void( uint8_t* frame, int32_t width, int32_t height)> callback) = 0;
    public:
        virtual void PlayerCreate(int32_t id, int32_t video_width,
            int32_t video_height,
            int32_t command_line_argument_count,
            const char** command_line_arguments) = 0;

        virtual void PlayerDispose(int32_t id) = 0;

        virtual void PlayerOpen(int32_t id, bool auto_start, const char** source,
            int32_t source_size) = 0;

        virtual void PlayerPlay(int32_t id) = 0;

        virtual void PlayerPause(int32_t id) = 0;

        virtual void PlayerPlayOrPause(int32_t id) = 0;

        virtual void PlayerStop(int32_t id) = 0;

        virtual void PlayerNext(int32_t id) = 0;

        virtual void PlayerPrevious(int32_t id) = 0;

        virtual void PlayerJumpToIndex(int32_t id, int32_t index) = 0;

        virtual void PlayerSeek(int32_t id, int32_t position) = 0;

        virtual void PlayerSetVolume(int32_t id, float volume) = 0;

        virtual void PlayerSetRate(int32_t id, float rate) = 0;

        virtual void PlayerSetUserAgent(int32_t id, const char* user_agent) = 0;

        virtual void PlayerSetDevice(int32_t id, const char* device_id,
            const char* device_name) = 0;

        virtual void PlayerSetEqualizer(int32_t id, int32_t equalizer_id) = 0;

        virtual void PlayerSetPlaylistMode(int32_t id, const char* mode) = 0;

        virtual void PlayerAdd(int32_t id, const char* type, const char* resource) = 0;

        virtual void PlayerRemove(int32_t id, int32_t index) = 0;

        virtual void PlayerInsert(int32_t id, int32_t index, const char* type,
            const char* resource) = 0;

        virtual void PlayerMove(int32_t id, int32_t initial_index,
            int32_t final_index) = 0;

        virtual void PlayerTakeSnapshot(int32_t id, const char* file_path,
            int32_t width, int32_t height) = 0;

        virtual void PlayerSetAudioTrack(int32_t id, int32_t track) = 0;

        virtual int32_t PlayerGetAudioTrackCount(int32_t id) = 0;

        virtual void PlayerSetHWND(int32_t id, int64_t hwnd) = 0;

        virtual const char** MediaParse(const char* type,
            const char* resource, int32_t timeout) = 0;

        virtual void BroadcastCreate(int32_t id, const char* type,
            const char* resource, const char* access,
            const char* mux, const char* dst,
            const char* vcodec, int32_t vb,
            const char* acodec, int32_t ab) = 0;

        virtual void BroadcastStart(int32_t id) = 0;

        virtual void BroadcastDispose(int32_t id) = 0;

        virtual void ChromecastCreate(int32_t id, const char* type,
            const char* resource, const char* ip_address) = 0;

        virtual void ChromecastStart(int32_t id) = 0;

        virtual void ChromecastDispose(int32_t id) = 0;

        virtual void RecordCreate(int32_t id, const char* saving_file,
            const char* type, const char* resource) = 0;

        virtual void RecordStart(int32_t id) = 0;

        virtual void RecordDispose(int32_t id) = 0;

        virtual DartDeviceList* DevicesAll() = 0;

        virtual struct DartEqualizer* EqualizerCreateEmpty() = 0;

        virtual struct DartEqualizer* EqualizerCreateMode(
            int32_t mode) = 0;

        virtual void EqualizerSetBandAmp(int32_t id, float band, float amp) = 0;

        virtual void EqualizerSetPreAmp(int32_t id, float amp) = 0;
    };
}