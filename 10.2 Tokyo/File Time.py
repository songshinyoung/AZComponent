import os
import time
import datetime
import ctypes
from ctypes import wintypes

def set_creation_time(path, timestamp):
    # Windows API를 사용해 생성 시간 변경
    FILE_WRITE_ATTRIBUTES = 0x0100
    OPEN_EXISTING = 3

    kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)
    CreateFileW = kernel32.CreateFileW
    CreateFileW.argtypes = [
        wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD,
        wintypes.LPVOID, wintypes.DWORD, wintypes.DWORD, wintypes.HANDLE
    ]
    CreateFileW.restype = wintypes.HANDLE

    hfile = CreateFileW(path, FILE_WRITE_ATTRIBUTES, 0, None, OPEN_EXISTING, 0, None)
    if hfile == wintypes.HANDLE(-1).value:
        print(f"[오류] 파일 열기 실패: {path}")
        return

    # FILETIME 구조체로 변환 (100나노초 단위로 1601년부터)
    ft = int((timestamp + 11644473600) * 10000000)
    low = ft & 0xFFFFFFFF
    high = ft >> 32

    creation_time = wintypes.FILETIME(low, high)
    # 수정 시간 및 엑세스 시간은 None으로 두면 유지됨
    success = kernel32.SetFileTime(hfile, ctypes.byref(creation_time), None, None)
    kernel32.CloseHandle(hfile)

    if not success:
        print(f"[오류] 생성 시간 설정 실패: {path}")

def update_file_timestamps(root_folder):
    now = time.time()
    print(f"현재 시간: {datetime.datetime.fromtimestamp(now)}")

    for dirpath, _, filenames in os.walk(root_folder):
        for name in filenames:
            full_path = os.path.join(dirpath, name)
            try:
                # 수정 시간, 접근 시간 설정
                os.utime(full_path, (now, now))
                # 생성 시간 설정
                set_creation_time(full_path, now)
                print(f"✔ 변경됨: {full_path}")
            except Exception as e:
                print(f"[에러] {full_path}: {e}")

if __name__ == "__main__":
    current_directory = os.getcwd()
    update_file_timestamps(current_directory)
