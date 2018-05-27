#!/bin/bash
if [[ -z "${SHORTEN_PROMPT_ENV}" ]]; then
  SHORTEN_PROMPT_ENV=`pwd`;
  export SHORTEN_PROMPT_ENV;
fi
#export PROMPT_COMMAND='PS1="$(python `echo $SHORTEN_PROMPT_ENV`/.smart.bash.prompt.py)"'
export PROMPT_COMMAND='PS1="$(python $SHORTEN_PROMPT_ENV/.smart.bash.prompt.py)"'
