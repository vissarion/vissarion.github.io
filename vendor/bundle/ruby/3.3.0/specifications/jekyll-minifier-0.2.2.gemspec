# -*- encoding: utf-8 -*-
# stub: jekyll-minifier 0.2.2 ruby lib

Gem::Specification.new do |s|
  s.name = "jekyll-minifier".freeze
  s.version = "0.2.2".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["DigitalSparky".freeze]
  s.date = "1980-01-02"
  s.description = "Jekyll Minifier using htmlcompressor for html, terser for js, and cssminify2 for css".freeze
  s.email = ["matthew@spurrier.com.au".freeze]
  s.homepage = "http://github.com/digitalsparky/jekyll-minifier".freeze
  s.licenses = ["GPL-3.0-or-later".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 3.0.0".freeze)
  s.rubygems_version = "3.6.9".freeze
  s.summary = "Jekyll Minifier for html, css, and javascript".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 2

  s.add_runtime_dependency(%q<jekyll>.freeze, ["~> 4.0".freeze])
  s.add_runtime_dependency(%q<terser>.freeze, ["~> 1.2.3".freeze])
  s.add_runtime_dependency(%q<htmlcompressor>.freeze, ["~> 0.4".freeze])
  s.add_runtime_dependency(%q<cssminify2>.freeze, ["~> 2.1.0".freeze])
  s.add_runtime_dependency(%q<json-minify>.freeze, ["~> 0.0.3".freeze])
  s.add_development_dependency(%q<rake>.freeze, ["~> 13.3".freeze])
  s.add_development_dependency(%q<rspec>.freeze, ["~> 3.13".freeze])
  s.add_development_dependency(%q<jekyll-paginate>.freeze, ["~> 1.1".freeze])
  s.add_development_dependency(%q<redcarpet>.freeze, ["~> 3.4".freeze])
  s.add_development_dependency(%q<rss>.freeze, ["~> 0.3".freeze])
end
