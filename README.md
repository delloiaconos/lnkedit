# lnkedit

lnkedit allows editing of Windows shortcuts (.lnk files) from the commandline. Reasons for using lnkedit rather than right-clicking on the shortcut and selecting Properties:

* lnkedit does not enforce the requirement that the new target exists.
* Opening and editing lots of shortcuts is tiresome monkey work.

lnkedit requires Cygwin.

lnkedit is based on work by Iain Patterson.

### Description

lnkedit was written to address a problem I ran into using roaming profiles on a Windows 2000 domain.

One machine on the domain was installed on drive C. Another was installed on drive D. Thus many of the shortcuts in my personal start menu (%userprofile%/Start Menu/Programs) were wrong when I logged on to at least one of the machines. For example there might be a shortcut to C:/Program Files/WinRAR/winrar.exe which would be invalid on the machine where WinRAR was installed on drive D.

Windows exports the global environment variable %programfiles% but if software publishers expand environment strings when they install their shortcuts, it isn't much help.

Rather than spend hours laboriously opening and changing every shortcut in my profile, I wrote lnkedit to do it for me!

### Usage

lnkedit has three basic modes of operation. Either it can dump some information about a shortcut, conditionally dump information based on a regular expression match or replace information; again using regular expression matching.

An option argument string can be used to limit lnkedit's operation to an combination of the shortcut's target, working directory and arguments.

### Display shortcut information

When run with a single argument, lnkedit prints information describing a shortcut's target:

    /bin/bash:~$ lnkedit Photoshop\ 7.0.lnk
     Shortcut: Photoshop 7.0.lnk
       Target: P:\adobe\Photoshop 7.0\Photoshop.exe
     Arguments: 
       Start in: P:\adobe\Photoshop 7.0
 

### Showing shortcuts which match a regular expression

When lnkedit is passed a second argument, it interprets this as a case-insensitive regular expression and only dumps information about the shortcut if one of the three fields matches that regular expresion.

    /bin/bash:~$ lnkedit Photoshop\ 7.0.lnk adobe
     Shortcut: Photoshop 7.0.lnk
       Target: P:\adobe\Photoshop 7.0\Photoshop.exe
     Arguments: 
        Start in: P:\adobe\Photoshop 7.0

You can add optional flags to restrict lnkedit's search to any combination of the target, arguments and working directory fields.

The -t flag instructs lnkedit to look in the target field.

The -a flag instructs lnkedit to look in the arguments field.

The -d flag instructs lnkedit to look in the working directory (Start in) field.

To combine flags, add them as a single argument with one dash. For example, to search target and working directory, use -td.

The following example shows no match because "Adobe" is not found in the Photoshop shortcut's argument list.

    /bin/bash:~$ lnkedit -a Photoshop\ 7.0.lnk adobe
     Shortcut: Photoshop 7.0.lnk (no match)

### Replacing values in a shortcut

When a third argument is added, lnkedit replaces substrings which matching the regular expression with the provided text.

To solve the example at the beginning of this page, you could use:

    /bin/bash:~$ lnkedit WinRAR.lnk 'C:\Program Files' '%programfiles%'
	
In fact, shortcuts may use DOS 8.3 names. My batch conversion ultimately used:

    find -name \*.lnk -exec lnkedit {} '[a-z]:.Progra[^\]+' '%programfiles%' \;