#/****************************************************************************
#*                                                                           *
#*  NiTE Controls 1.x Alpha                                                  *
#*  Copyright (C) 2013 PrimeSense Ltd.                                       *
#*                                                                           *
#*  This file is part of NiTE Controls Lab.                                  *
#*                                                                           *
#*  Licensed under the Apache License, Version 2.0 (the "License");          *
#*  you may not use this file except in compliance with the License.         *
#*  You may obtain a copy of the License at                                  *
#*                                                                           *
#*      http://www.apache.org/licenses/LICENSE-2.0                           *
#*                                                                           *
#*  Unless required by applicable law or agreed to in writing, software      *
#*  distributed under the License is distributed on an "AS IS" BASIS,        *
#*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
#*  See the License for the specific language governing permissions and      *
#*  limitations under the License.                                           *
#*                                                                           *
#****************************************************************************/
import os
import re

PATTERN = "Method:\s*(\w*)(.*)(.*)"
#PATTERN += ".*(Signature:)\s*(.*)"
#PATTERN += ".*JNIEXPORT.*JNICALL (.*)"

compiled_re = re.compile(PATTERN, re.MULTILINE)

# ----------------------- MAIN -------------------------
java_header = open("com_primesense_NiteControls_NativeMethods.h")
cont = java_header.read()
java_header.close()
result = open("methods.inl", "w")
result.write("static JNINativeMethod methods[] = {\n")

while True:
    match = re.search("Method:\s*(\w*)", cont)
    if match is None:
        break
    method_name = match.group(1)
    
    match = re.search("Signature:\s*([\w\(\);/]*)", cont)
    if match is None:
        break
    signature = match.group(1)
        
    match = re.search("JNIEXPORT.*JNICALL (\w*)", cont)
    if match is None:
        break
    method = match.group(1)

    result.write('\t{ "' + method_name + '", "' + signature + '", (void*)&' + method + ' },\n')
    cont = cont[match.end():];

result.write('};\n');
result.close()
