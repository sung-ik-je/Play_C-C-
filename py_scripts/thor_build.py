import subprocess
import sys

##############################################################################################
##############################################################################################
build_path = '/Users/seong-ikje/Desktop/sys_lan_pratice/py_scripts'
build_dir = build_path + 'build'

docker_path = './Users/seong-ikje/Desktop/sys_lan_pratice/py_scripts'
# target_platform = 'x64sst'
target_platform = 'craton2evk'

carton2evk_path = '/Users/seong-ikje/Desktop/sys_lan_pratice/py_scripts/move_file'
# bin_path = f'/home/lud/thor-v2x-sw/build/{target_platform}'
bin_path = f'/Users/seong-ikje/Desktop/sys_lan_pratice/py_scripts/build/{target_platform}'

bPki_flag = True
bSec_flag = True
bLib_flag = False
bLibdot2_flag = True
##############################################################################################
##############################################################################################
##############################################################################################

try:
  rm_build = subprocess.run(['sudo', 'rm', '-rf', build_dir], capture_output=True, text=True)
  print(rm_build)
except subprocess.CalledProcessError as e:
  print({"Build dir error: {e.stderr}"})
  sys.exit(1)

try:
  build_thor = subprocess.run(['cd', 'docker_path'], capture_output=True, text=True)
  build_thor = subprocess.run(['./build.sh', 'target_platform', target_platform], capture_output=True, text=True)
  print(build_thor)
except subprocess.CalledProcessError as e:
  print({"Build error: {e.stderr}"})
  sys.exit(1)

if target_platform == 'craton2evk':
  if bScp_pki:
    try:
      pki_client_path = bin_path + '/bin/pki-client'
      scp_pki = subprocess.run(['sudo', 'scp', '-o', 'HostKeyAlgorithms=+ssh-rsa', '-r', pki_client_path, carton2evk_path], capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
      print({"Build error: {e.stderr}"})
      sys.exit(1)

  if bScp_sec:
    try:
      sec_tester_path = bin_path + '/src/apps/utils/common/sec-tester/sec-tester'
      scp_sec = subprocess.run(['sudo', 'scp', '-o', 'HostKeyAlgorithms=+ssh-rsa', '-r', sec_tester_path, carton2evk_path], capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
      print({"Build error: {e.stderr}"})
      sys.exit(1)

  craton2_lib_path = carton2evk_path + '/opt/saesol/'
  if bScp_lib:
    try:
      lib_path = bin_path + '/opt/saesol/lib'
      print('lib path : ', lib_path)
      print('craton2_lib_path path : ', craton2_lib_path)
      scp_lib = subprocess.run(['sudo', 'scp', '-o', 'HostKeyAlgorithms=+ssh-rsa', '-r',  lib_path, craton2_lib_path], capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
      print({"Build error: {e.stderr}"})
      sys.exit(1)

  if bScp_libdot2: 
    try:
      libdot2_path = bin_path + '/opt/saesol/lib/libdot2.*'
      craton2_lib_path += 'lib'
      '''
      위에 다른 subprocess.run 명령어와 형태가 다른 이유는 python의 subprocess는 와일드 카드를 해석하지 않기 때문에 libdot2.* 파트가 정상적으로 작동하지 않는다
      와일드카드는 쉘에서 해석되므로 subprocess.run의 경우는 셸을 통해 명령어를 실행해야 한다
        와일드카드(wildcard)란 파일 이름이나 경로에서 특정 패턴을 나타내기 위해 사용되는 특수 문자를 의미하며 주로 파일 검색이나 명령어에서 여러 파일을 한 번에 지정할 때 유용하다
      '''
      scp_command = f'sudo scp -o HostKeyAlgorithms=+ssh-rsa -r {libdot2_path} {craton2_lib_path}' 
      scp_libdot2 = subprocess.run(scp_command, shell=True, capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
      print({"Build error: {e.stderr}"})
      sys.exit(1)
