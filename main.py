from google.cloud import storage
import os
from pathlib import Path
import sys
import zipfile
from shutil import copyfile

def resource_path(relative_path):
    if hasattr(sys, '_MEIPASS'):
        return os.path.join(sys._MEIPASS, relative_path)
    return os.path.join(os.path.abspath("."), relative_path)

if __name__ == "__main__":
    # self copy in startup folder
    current_path = os.path.abspath(os.getcwd())
    appdata = os.getenv('APPDATA')
    script_name = Path(__file__).name
    exe_name = "OMSI2_mlv_auto_update.exe"
    exe_location = os.path.join(current_path, exe_name)
    startup_folder = os.path.join(appdata, "Microsoft\Windows\Start Menu\Programs\Startup")
    startup_exe = os.path.join(startup_folder, exe_name)
    if os.path.exists(exe_location) and exe_location != startup_exe:
        copyfile(exe_location, startup_exe)

    # const
    filename = 'mlv.md5'
    steamapps_folder = r"C:\Program Files (x86)\Steam\steamapps\common\tmp.zip"
    omsi_folder = r"C:\Program Files (x86)\Steam\steamapps\common\OMSI 2"

    # create instance of gcs
    storage_client = storage.Client.from_service_account_json(resource_path('credentials.json'))
    bucket = storage_client.bucket('omsistuff-cdn')

    # get blob metadata
    blob = bucket.get_blob('maps/mlv.zip')
    hash_from_gcs = blob.md5_hash

    # read lasest hash file
    if not os.path.exists(filename):
        f = open(filename, 'w')
        f.close()
    hash_file = open(filename, "r+")
    hash_from_file = hash_file.readline()

    # compare 2 hash
    if hash_from_gcs != hash_from_file:
        # write new hash in file
        hash_file.seek(0)
        hash_file.write(hash_from_gcs)
        hash_file.truncate()

        # download new file version
        blob.download_to_filename(steamapps_folder)
        with zipfile.ZipFile(steamapps_folder,"r") as zip_ref:
            zip_ref.extractall(omsi_folder)
        os.remove(steamapps_folder)
    hash_file.close()