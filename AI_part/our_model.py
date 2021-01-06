import os
import glob
import numpy as np
import librosa
import keras

# wav 파일 전처리 함수
def extract_feature(file_name):
    X, sample_rate = librosa.load(file_name)
    stft = np.abs(librosa.stft(X))
    mfccs = np.mean(librosa.feature.mfcc(y=X, sr=sample_rate, n_mfcc=40).T,axis=0)
    chroma = np.mean(librosa.feature.chroma_stft(S=stft, sr=sample_rate).T,axis=0)
    mel = np.mean(librosa.feature.melspectrogram(X, sr=sample_rate).T,axis=0)
    contrast = np.mean(librosa.feature.spectral_contrast(S=stft, sr=sample_rate).T,axis=0)
    tonnetz = np.mean(librosa.feature.tonnetz(y=librosa.effects.harmonic(X), sr=sample_rate).T,axis=0)
    return mfccs,chroma,mel,contrast,tonnetz

# wav 파일 경로 지정 및 전처리 함수 불러오기
def parse_audio_files(file_path):
    features = np.empty((0,193)) # 193 => total features 

    mfccs, chroma, mel, contrast,tonnetz = extract_feature(file_path)

    ext_features = np.hstack([mfccs,chroma,mel,contrast,tonnetz])
    features = np.vstack([features,ext_features])

    return np.array(features, dtype=np.float32)

# 예측 함수
def predict(w_array):
  ourmodel = keras.models.load_model('/content/drive/MyDrive/VitA_model_save/fnn_9.h5') # h5 파일 경로
  result = np.argmax(ourmodel.predict(w_array), axis=-1)
  # print(result)

  return result