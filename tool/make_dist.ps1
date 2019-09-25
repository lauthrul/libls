##############################
# 参数定义
# 原位置
$src_dir = "..\libls\src\", "..\libls_util\src\"
$src_headers =  "core\crypto\crypto.h",
                "core\json\json_x.h",
                "core\lock\lock.h",
                "core\logger\logger.h",
                "core\math\math.h",
                "core\net\httpclient.h",
                "core\os\os.h",
                "core\process\process.h",
                "core\registry\registry.h",
                "core\string\strtool.h",
                "core\task\TaskAgent.h",
                "core\task\TaskDefine.h",
                "core\task\TaskManager.h",
                "core\thread\idle_thread.h",
                "core\thread\thread.h",
                "core\time\timer.h",
                "core\xml\xml.h",
                "core\zip\zip_x.h",
                "core\mmr.h",
                "core\type.h",
                "utils\cfghandler.h",
                "utils\cmdline.h",
                "utils\utils.h",
                "libls.h",
                "json\autolink.h",
                "json\config.h",
                "json\features.h",
                "json\forwards.h",
                "json\json.h",
                "json\reader.h",
                "json\value.h",
                "json\writer.h",
                "xml\tinystr.h",
                "xml\tinyxml.h"
                
$lib_dir = "..\libls\bin\", "..\libls_util\bin\"
$lib_files = "libls.dll", "libls.lib", "libls.pdb", "libls_util.lib"

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
foreach ($d in $src_dir) {
    get-childitem -recurse $d -filter "*.h" | foreach-object -process {
        $name = $_.fullname.replace((resolve-path $d), "")
        #write-host $name
        if ($src_headers -contains $name) {
            $target = $include_path + $name
            #write-host $target
            copyfile $_.fullname $target
        }
    }
}

# 拷贝lib文件
write-host "=== copy libs ==="
mkdir $lib_path | out-null
foreach ($d in $lib_dir) {
    get-childitem -recurse $d | foreach-object -process {
        if ($lib_files -contains $_.name) {
            $name = $_.fullname.replace((resolve-path $d), "")
            #write-host $name        
            $target = $lib_path + $name
            #write-host $target
            copyfile $_.fullname $target
        }
    }
}