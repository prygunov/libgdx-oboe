#include "audio_decoder.hpp"

audio_decoder::audio_decoder(jni_context p_context, AssetFileDescriptor p_asset_fd)
    : m_context(p_context)
    , m_decoder_class(p_context, "barsoosayque/libgdxoboe/AudioDecoder")
    , m_decoder_object(m_decoder_class.construct(p_asset_fd)) { }

audio_decoder::~audio_decoder() {
    m_decoder_class.execute_method<void()>(m_decoder_object, "dispose");
}

std::vector<int16_t> as_vector(const jni_context& p_context, const jbyteArray p_array) {
    int length = p_context->GetArrayLength(p_array);
    int16_t* pointer = reinterpret_cast<int16_t*>(p_context->GetByteArrayElements(p_array, NULL));
    std::vector<int16_t> v;
    if (length > 0) {
        v.insert(v.end(), std::make_move_iterator(pointer), std::make_move_iterator(pointer + length / 2)) ;
    }
    return v;
}

std::vector<int16_t> audio_decoder::decode(int p_samples) {
    auto buffer = m_decoder_class.execute_method<jbyteArray(int)>(m_decoder_object, "decode", p_samples);
    return as_vector(m_context, buffer);
}

std::vector<int16_t> audio_decoder::decode() {
    auto buffer = m_decoder_class.execute_method<jbyteArray()>(m_decoder_object, "decode");
    return as_vector(m_context, buffer);
}

void audio_decoder::seek(float p_seconds) {
    m_decoder_class.execute_method<void(float)>(m_decoder_object, "seek", p_seconds);
}