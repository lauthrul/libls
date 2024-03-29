##############################
# 参数定义
# 原位置
$src_projects = "libls", "libls_util"
$src_dir_fmt = "..\{}\src\"
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

$lib_root = "..\out\"
$lib_projects = "libls", "libls_util"
$lib_files = "libls.dll", "libls.lib", "libls.pdb", "libls_util.lib"

##############################
#目标位置
$target_dir = "..\dist\"
if (($args.Count -ge 1) -and ($args[0] -eq "install")) {
    <#$target_dir = [environment]::getenvironmentvariable("programfiles")
    if ($args.Count -ge 2) {
        $target_dir = $args[1]
    }
    $target_dir += "\libls\"#>
    $target_dir = "D:\"
}
$include_path = $target_dir + "include\"
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
foreach ($d in $src_projects) {
    $src_dir = $src_dir_fmt -replace "{}",$d
    #write-host $src_dir
    get-childitem -recurse $src_dir -filter "*.h" | foreach-object -process {
        $name = $_.fullname.replace((resolve-path $src_dir), "")
        #write-host $name
        if ($src_headers -contains $name) {
            $target = $include_path + $d + "\" + $name
            #write-host $target
            copyfile $_.fullname $target
        }
    }
}

# 拷贝lib文件
write-host "=== copy libs ==="
mkdir $lib_path | out-null
foreach ($d in $lib_projects) {
    $lib_dir = $lib_root + $d
    #write-host $lib_dir
    get-childitem -recurse $lib_dir | foreach-object -process {
        if ($lib_files -contains $_.name) {
            #write-host $_.fullname
            $name = $_.fullname.replace((resolve-path $lib_root), "")
            #write-host $name
            $target = $lib_path + $name
            #write-host $target
            copyfile $_.fullname $target
        }
    }
}