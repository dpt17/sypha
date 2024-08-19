# Usage
backup.sh -i <dir_to_backup> -o <dir_to_write_to> -d

A simple wrapper for rsync where "-d" indicates if files in the output directory that DON'T exist in the input directory
should be purged.  Note: each directory argument is resolved to an absolute path on the system.

# Run in cron
crontab -e

00 07 * * * /path/to/backup/backup.sh -i /Back/Me/Up/Dir -o /Write/Me/To/Here/Dir -d

# Potential Use Cases

1) Syncing one folder to another
