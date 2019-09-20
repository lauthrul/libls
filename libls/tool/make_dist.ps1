##############################
# 参数定义
# 原位置
$src_dir = "..\src\"
$exclude_headers = "stdafx.h",
                "targetver.h",
                "core\crypto\aes.h",
                "core\crypto\base64.h",
                "core\crypto\cipher_def.h",
                "core\crypto\des.h",
                "core\crypto\md5.h",
                "core\crypto\sha1.h",
                "core\crypto\sha256.h",
                "core\crypto\url_encode.h",
                "core\json\json_batchallocator.h",
                "core\string\string.h"

$src_headers =  "core\crypto\crypto.h",
                "core\json\autolink.h",
                "core\json\config.h",
                "core\json\features.h",
                "core\json\forwards.h",
                "core\json\json.h",
                "core\json\json_x.h",
                "core\json\reader.h",
                "core\json\value.h",
                "core\json\writer.h",
                "core\lock\lock.h",
                "core\logger\logger.h",
                "core\math\math.h",
                "core\net\httpclient.h",
                "core\os\os.h",
                "core\process\process.h",
                "core\registry\registry.h",
                "core\string\strtool.h",
                "core\thread\idle_thread.h",
                "core\thread\thread.h",
                "core\time\timer.h",
                "core\xml\tinystr.h",
                "core\xml\tinyxml.h",
                "core\xml\xml.h",
                "core\zip\zip_x.h",
                "core\mmr.h",
                "core\type.h",
                "utils\cfghandler.h",
                "utils\cmdline.h",
                "utils\utils.h",
                "libls.h"

$lib_dir = "..\bin\"
$lib_files = "libls.dll", "libls.lib", "libls.pdb"

##############################
#目标位置
$target_dir = "..\dist\"
if (($args.Count -ge 1) -and ($args[0] -eq "install")) {
    $target_dir = [environment]::getenvironmentvariable("programfiles")
    if ($args.Count -ge 2) {
        $target_dir = $args[1]
    }
    $target_dir += "\libls\"
}
$include_path = $target_dir + "include\libls\"
$lib_path = $target_dir + "lib\"

write-host target: $target_dir

##############################
# 函数定义
function mkdir([string]$path) {
    if(-not (Test-Path -Path $path)){
        New-Item -Path $path -ItemType Directory
    }
}

function copyfile([string]$src, [string]$target) {
    write-host  "$src ==> $target"
    $dir = $target.substring(0, $target.lastindexof("\"))
    mkdir($dir) | out-null
    copy-item -path $src -destination $dir | out-null
}

##############################
# 脚本
# 拷贝头文件
write-host "=== copy headers ==="
mkdir $include_path | out-null
get-childitem -recurse $src_dir -filter "*.h" | foreach-object -process {
    $name = $_.fullname.replace((resolve-path $src_dir), "")
    #write-host $name
    if ($src_headers -contains $name) {
        $target = $include_path + $name
        #write-host $target
        copyfile $_.fullname $target
    }
}

# 拷贝lib文件
write-host "=== copy libs ==="
mkdir $lib_path | out-null
get-childitem -recurse $lib_dir | foreach-object -process {
    if ($lib_files -contains $_.name) {
        $name = $_.fullname.replace((resolve-path $lib_dir), "")
        #write-host $name        
        $target = $lib_path + $name
        #write-host $target
        copyfile $_.fullname $target
    }
}