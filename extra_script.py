Import("env")
import os

def before_build_spiffs(source, target, env):
    print("Building React App...")
    env.Execute("cd web && npm run build")
    print("React App built!") 

env.AddPreAction("$BUILD_DIR/spiffs.bin", before_build_spiffs)