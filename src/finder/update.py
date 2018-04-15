import ctypes, sys
import os, shutil
import sys
import urllib3
import tempfile
import os
import subprocess
import zipfile


def is_admin():
	try:
		return ctypes.windll.shell32.IsUserAnAdmin()
	except:
		return False
		
def extract_archive(filename):
	zip_ref = zipfile.ZipFile(filename, 'r')
	zip_ref.extractall()
	zip_ref.close()
	
def get_update_file():
	root = os.environ["LOCALAPPDATA"]
	folder_name = 'Fetch'
	path = os.path.join(root, folder_name)
	return os.path.join(path, 'update.zip')
	
def main():
	print("Updating Fetch...")
	try:
		f = get_update_file()
		extract_archive(f)
	finally:
		subprocess.Popen("Fetch.exe")

if is_admin():
	main()
else:
	ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, None, None, 1)