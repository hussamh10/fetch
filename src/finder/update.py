import sys
import urllib3
import tempfile
import os
import subprocess
import zipfile

def get_update_url():
	return "https://raw.githubusercontent.com/hussamh10/fetch/gh-pages/update.zip"
	
def extract_archive(filename, extract_path):
	zip_ref = zipfile.ZipFile(filename, 'r')
	zip_ref.extractall(extract_path)
	zip_ref.close()
	
def download(url, method='GET'):
	http = urllib3.PoolManager()
	r = http.request(method, url)
	return r.data
	
def download_update(f):
	f.write(download(url=get_update_url()))
	f.close()
	return f.name
	
def get_fetch_path():
	return ' '.join(sys.argv[1:])
	
def main():
	print(get_fetch_path())
	try:
		f = download_update(tempfile.NamedTemporaryFile(delete=False))
		extract_archive(f, get_fetch_path())
	finally:
		os.chdir(get_fetch_path())
		subprocess.Popen("Fetch.exe")

urllib3.disable_warnings()
main()