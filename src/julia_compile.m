function julia_compile(julia_config_path)
	FILES = {'julia_eval.c', 'julia_call.c'};
	lastWorkingDir = pwd;

	cd(mfiledir);
	c = onCleanup(@() cd(lastWorkingDir));

	if ~isfile(julia_config_path)
		error([mfilename ':FileNotFound'], 'Julia configuration file not found at %s.', julia_config_path);
	end

	cflags = regexp(julia_config_call(julia_config_path, '--cflags'), '\S+', 'match');
	ldflags = regexp(julia_config_call(julia_config_path, '--ldflags'), '\S+', 'match');
	ldlibs = regexp(julia_config_call(julia_config_path, '--ldlibs'), '[\S ]+', 'match', 'once');

	options = [cflags{:} ldflags{:} {sprintf('CFLAGS="\\$CFLAGS %s"', ldlibs)}];

	for i = 1:numel(FILES)
		mex(fullfile(mfiledir, FILES{i}), fullfile(mfiledir, 'julia_common.c'), options{:} );
	end
end

function out = julia_config_call(julia_config_path, flag)
	[rv, out, err] = system([julia_config_path ' ' flag]);

	if rv ~= 0
		[~, name, ext] = fileparts(julia_config_path);
		error([name ext ':Error'], '%s', err);
	end
end